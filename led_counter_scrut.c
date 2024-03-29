#include "config_bits.h"

// peripheral clock frequency
#define BASE_TIMER_FREQ 120000000

// masks for 7 segs displays
#define MASK_SEG_A (1<<12)
#define MASK_SEG_B (1<<14)
#define MASK_SEG_C (1<< 6)
#define MASK_SEG_D (1<<13)
#define MASK_SEG_E (1<<15)
#define MASK_SEG_F (1<< 7)
#define MASK_SEG_G (1<<13)
#define MASK_POINT (1<<14)
#define MASK_AN0   (1<<12)
#define MASK_AN1   (1<<13)
#define MASK_AN2   (1<< 9)
#define MASK_AN3   (1<<10)

// indexed access to mask and register of 7 seg display anodes
const uint32_t  SEG7_AFF_MASK[4] = {MASK_AN0, MASK_AN1, MASK_AN2, MASK_AN3};
volatile uint32_t *SEG7_AFF_LAT[4] = {&LATB,   &LATB,   &LATA,   &LATA};

// indexed access to mask and register of 7 seg display segemnts
// 0:seg a, 1:seg b, .... 6:seg g, 7:point
const uint32_t  SEG7_SEG_MASK[8] = {MASK_SEG_A, MASK_SEG_B, MASK_SEG_C, MASK_SEG_D, MASK_SEG_E, MASK_SEG_F, MASK_SEG_G, MASK_POINT};
volatile uint32_t  *SEG7_SEG_LAT[8] = {&LATG,   &LATA,   &LATD,   &LATG, &LATG,   &LATD,   &LATD,   &LATG};

// digit representation expressed in 7 seg : bit 0 : seg a, ... , bit 6 : seg g, bit 7 : point
const char digit_7seg[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};



void wait_timer1() {
    // this function blocks until timer 1 expires
    while ((IFS0 & (1 << 4)) == 0) {}
    IFS0 = IFS0 & ~(1 << 4);
}

void wait_timer2() {
    // this function blocks until timer 2 expires
    while ((IFS0 & (1 << 9)) == 0) {}
    IFS0 = IFS0 & ~(1 << 9);
}

void wait_timer3() {
    // this function blocks until timer 3 expires
    while ((IFS0 & (1 << 14)) == 0) {}
    IFS0 = IFS0 & ~(1 << 14);
}

void sleep() {
    // go to sleep 
    asm volatile("wait");

}


void init_7seg(void) {
    ANSELB = ANSELB & ~(MASK_AN0 | MASK_AN1);                   // inhibit analog functions on B12 and B13
    TRISB  = TRISB  & ~(MASK_AN0 | MASK_AN1);                   // set output mode for B12, B13
    TRISA  = TRISA  & ~(MASK_AN2 | MASK_AN3 | MASK_SEG_B);      // set output mode for A9,  A10, A14
    TRISG  = TRISG  & ~(MASK_SEG_A | MASK_SEG_D | MASK_SEG_E | MASK_POINT);   // set output mode for G12, G13, G14, G15
    TRISD  = TRISD  & ~(MASK_SEG_C | MASK_SEG_F | MASK_SEG_G);  // set output mode for D6,  D7,  D13

    LATB = LATB | MASK_AN0 | MASK_AN1; 
    LATA = LATA | MASK_AN2 | MASK_AN3;

    LATG = LATG | MASK_SEG_A; // set G12 : SEG A = 1 (off)
    LATA = LATA | MASK_SEG_B; // set A14 : SEG B = 1 (off)
    LATD = LATD | MASK_SEG_C; // set D6  : SEG C = 1 (off)
    LATG = LATG | MASK_SEG_D; // set G13 : SEG D = 1 (off)
    LATG = LATG | MASK_SEG_E; // set G15 : SEG E = 1 (off)
    LATD = LATD | MASK_SEG_F; // set D7  : SEG F = 1 (off)
    LATD = LATD | MASK_SEG_G; // set D13 : SEG G = 1 (off)
    LATG = LATG | MASK_POINT; // set G14 : POINT = 1 (off)

}



void main() {
    // variables
    char displayed_7seg = 0;         // to memorize which display is considered
    char seg_map[4] = {0, 0, 0, 0};  // to memorize segments of each display
    char precounter = 10;            // counter predivision
    int  counter=0;                  // the counter to display
    int  counter_cpy;                // temporary copy of the counter
    char i;                          // loop counter    
    
    // Initialization
    init_7seg();

    TRISA = TRISA & 0xFF00; // set LEDs as output
    LATA &= 0xFF00;         // clear all LEDs

    // Timer 1 initialization @ 400 Hz
    T1CON = 0x8010; // set prescaler : /8
    PR1 = BASE_TIMER_FREQ/(8*400);

    // Timer 2 initialization @ 10 Hz
    T2CON = 0x8070; // set prescaler : /256
    PR2 = BASE_TIMER_FREQ/(256*10);
    
    // Timer 3 initialization @ 8 Hz
    T3CON = 0x8070;  // set prescaler : /256
    PR3 = BASE_TIMER_FREQ/(256*8);

    
    // loop
    while(1)
    {
        if (IFS0 & (1 << 4)) {  // if timer1 has expired
            // this portion of the code is entered each time timer1 expires,
            // so each 2.5 ms. It is used to switch from a display to the next
            // one
            
            // reset timer1 flag
            IFS0 = IFS0 & ~(1 << 4);

            // clear all 7seg display
            LATA |= MASK_AN2   | MASK_AN3   | MASK_SEG_B;
            LATB |= MASK_AN0   | MASK_AN1; 
            LATD |= MASK_SEG_C | MASK_SEG_F | MASK_SEG_G;
            LATG |= MASK_SEG_A | MASK_SEG_D | MASK_SEG_E | MASK_POINT;

            // update the display to be powered
            displayed_7seg = (displayed_7seg + 1) & 0x3;

            // light on selected display
            *(SEG7_AFF_LAT[displayed_7seg]) &= ~SEG7_AFF_MASK[displayed_7seg];
            
            // light on selected segments
            for (i=0; i<8; i++) {
                if (seg_map[displayed_7seg]>>i & 1) {
                    *(SEG7_SEG_LAT[i]) &= ~SEG7_SEG_MASK[i];
                }
            }
        }
 
        if (IFS0 & (1 << 9)) {  // if timer2 has expired
            // this portion of code is entered each 100 ms. it is used to update
            // the counter value and its 7seg representation
            
            // reset timer2 flag
            IFS0 = IFS0 & ~(1 << 9);

            if (--precounter == 0) {
                // events run at 10Hz, so we have to divide by 10.
                precounter = 10;
            
                // increment counter modulo 10000.
                // FIXME : is testing faster than computing a modulo ???
                if (++counter >= 10000) counter=0;

                // updates seg_map according to counter
                counter_cpy = counter;
                for (i=0; i<4; i++) {
                    seg_map[i] = digit_7seg[counter_cpy%10];
                    counter_cpy /= 10;
                }
            }
        }

        // place to put code for heartbeat...
        
        
    }
}