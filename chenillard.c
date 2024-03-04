#include "config_bits.h"

// #define MASK_SEG_A (1<<12)
// #define MASK_SEG_B (1<<14)
// #define MASK_SEG_C (1<< 6)
// #define MASK_SEG_D (1<<13)
// #define MASK_SEG_E (1<<15)
// #define MASK_SEG_F (1<< 7)
// #define MASK_SEG_G (1<<13)
// #define MASK_POINT (1<<14)
// #define MASK_AN0   (1<<12)
// #define MASK_AN1   (1<<13)
// #define MASK_AN2   (1<< 9)
// #define MASK_AN3   (1<<10)

#define MASK_LD7 (1<<7)
#define MASK_LD6 (1<<6)
#define MASK_LD5 (1<<5)
#define MASK_LD4 (1<<4)
#define MASK_LD3 (1<<3)
#define MASK_LD2 (1<<2)
#define MASK_LD1 (1<<1)
#define MASK_LD0 (1)
#define MASK_SW0 (1 << 3)
#define MASK_BUTTON (1<<8)


// the wait function
void wait(int l) {
    for (int i=0; i<l; i++) {
    // verfication du button
    }
}

int b8_state() {
    if ((PORTBbits.RB8) == 0) return 0;
    return 1; // button high
}

// Do I really need to precise that this is the main function ???
void main() {

// // CODE TP1
//     TRISA = 0x00;
//     LATA = 0xFF;



// OLD PROR    
// // Initialization
//     char fsm  = 0;                                              // to store the animation step
//     ANSELB = ANSELB & ~(MASK_AN0 | MASK_AN1);                   // inhibit analog functions on B12 and B13
//     TRISB  = TRISB  & ~(MASK_AN0 | MASK_AN1);                   // set output mode for B12, B13
//     TRISA  = TRISA  & ~(MASK_AN2 | MASK_AN3 | MASK_SEG_B);      // set output mode for A9,  A10, A14
//     TRISG  = TRISG  & ~(MASK_SEG_A | MASK_SEG_D | MASK_SEG_E | MASK_POINT);   // set output mode for G12, G13, G14, G15
//     TRISD  = TRISD  & ~(MASK_SEG_C | MASK_SEG_F | MASK_SEG_G);  // set output mode for D6,  D7,  D13
    
// // loop
//     while(1)
//     {
//         wait(50000);          // just wait !
//         fsm = (fsm+1)%12;     // next animation step
//         LATB = LATB | MASK_AN0; // set B12 (AN0=1, display 0 is off)
//         LATB = LATB | MASK_AN1; // set B13 (AN1=1, display 1 is off)
//         LATA = LATA | MASK_AN2; // set A9  (AN2=1, display 2 is off)
//         LATA = LATA | MASK_AN3; // set A10 (AN3=1, display 3 is off)

//         LATG = LATG | MASK_SEG_A; // set G12 : SEG A = 1 (off)
//         LATA = LATA | MASK_SEG_B; // set A14 : SEG B = 1 (off)
//         LATD = LATD | MASK_SEG_C; // set D6  : SEG C = 1 (off)
//         LATG = LATG | MASK_SEG_D; // set G13 : SEG D = 1 (off)
//         LATG = LATG | MASK_SEG_E; // set G15 : SEG E = 1 (off)
//         LATD = LATD | MASK_SEG_F; // set D7  : SEG F = 1 (off)
//         LATD = LATD | MASK_SEG_G; // set D13 : SEG G = 1 (off)
//         LATG = LATG | MASK_POINT; // set G14 : POINT = 1 (off)

//         switch (fsm) {
//             case 0 :
//                 LATB = LATB & ~(MASK_AN0);   // clear B12 (AN0=0, display 0 is on)
//                 LATG = LATG & ~(MASK_SEG_A); // clear G12 : SEG A = 0 (on)
//                 break;
//             case 1 :
//                 LATB = LATB & ~(MASK_AN1);   // clear B13 (AN1=0, display 1 is on)
//                 LATG = LATG & ~(MASK_SEG_A); // clear G12 : SEG A = 0 (on)
//                 break;
//             case 2 :
//                 LATA = LATA & ~(MASK_AN2);   // clear A9  (AN2=0, display 2 is on)
//                 LATG = LATG & ~(MASK_SEG_A); // clear G12 : SEG A = 0 (on)
//                 break;
//             case 3 :
//                 LATA = LATA & ~(MASK_AN3);   // clear A10 (AN3=0, display 3 is on)
//                 LATG = LATG & ~(MASK_SEG_A); // clear G12 : SEG A = 0 (on)
//                 break;
//             case 4 :
//                 LATA = LATA & ~(MASK_AN3);   // clear A10 (AN3=0, display 3 is on)
//                 LATD = LATD & ~(MASK_SEG_F); // clear D7 : SEG F = 0 (on)
//                 break;
//             case 5 :
//                 LATA = LATA & ~(MASK_AN3);   // clear A10 (AN3=0, display 3 is on)
//                 LATG = LATG & ~(MASK_SEG_E); // clear G15 : SEG E = 0 (on)
//                 break;
//             case 6 :
//                 LATA = LATA & ~(MASK_AN3);   // clear A10 (AN3=0, display 3 is on)
//                 LATG = LATG & ~(MASK_SEG_D); // clear G13 : SEG D = 0 (on)
//                 break;
//             case 7 :
//                 LATA = LATA & ~(MASK_AN2);   // clear A9  (AN2=0, display 2 is on)
//                 LATG = LATG & ~(MASK_SEG_D); // clear G13 : SEG D = 0 (on)
//                 break;
//             case 8 :
//                 LATB = LATB & ~(MASK_AN1);   // clear B13 (AN1=0, display 1 is on)
//                 LATG = LATG & ~(MASK_SEG_D); // clear G13 : SEG D = 0 (on)
//                 break;
//             case 9 :
//                 LATB = LATB & ~(MASK_AN0);   // clear B12 (AN0=0, display 0 is on)
//                 LATG = LATG & ~(MASK_SEG_D); // clear G13 : SEG D = 0 (on)
//                 break;
//             case 10 :
//                 LATB = LATB & ~(MASK_AN0);   // clear B12 (AN0=0, display 0 is on)
//                 LATD = LATD & ~(MASK_SEG_C); // clear D6 : SEG C = 0 (on)
//                 break;
//             case 11 :
//                 LATB = LATB & ~(MASK_AN0);   // clear B12 (AN0=0, display 0 is on)
//                 LATA = LATA & ~(MASK_SEG_B); // clear A14 : SEG B = 0 (on)
//                 break;
//         }
        
//     }


// Initialization
    char fsm  = 0;                                              // to store the animation step
    // ANSELB = ANSELB & ~(MASK_AN0 | MASK_AN1);                   // inhibit analog functions on B12 and B13
    // TRISB  = TRISB  & ~(MASK_AN0 | MASK_AN1);                   // set output mode for B12, B13
    // TRISA  = TRISA  & ~(MASK_AN2 | MASK_AN3 | MASK_SEG_B);      // set output mode for A9,  A10, A14
    // TRISG  = TRISG  & ~(MASK_SEG_A | MASK_SEG_D | MASK_SEG_E | MASK_POINT);   // set output mode for G12, G13, G14, G15
    // TRISD  = TRISD  & ~(MASK_SEG_C | MASK_SEG_F | MASK_SEG_G);  // set output mode for D6,  D7,  D13
    TRISA = 0x00;
    TRISF = TRISF | MASK_SW0;
    TRISB = TRISB | MASK_BUTTON; // input
    int time_to_wait_0 = 500000;
    int time_to_wait_1 = 50000;
    int current_time_to_wait = time_to_wait_0;
    int direction = 1;

    
// loop
    while(1)
    {
        if ((PORTF & MASK_SW0) == 0) {
            // Button high
            current_time_to_wait = time_to_wait_0;
        } else {
            current_time_to_wait = time_to_wait_1;
        }

        wait(current_time_to_wait);          // just wait !

        if (!b8_state) direction = -1;
        direction = 1;

        fsm = (fsm+direction)%7;     // next animation step
        
        switch (fsm) {
            case 0 :
                LATA = MASK_LD0;
                break;
            case 1 :
                LATA = MASK_LD1;
                break;
            case 2 :
                LATA = MASK_LD2;
                break;
            case 3 :
                LATA = MASK_LD3;
                break;
            case 4 :
                LATA = MASK_LD4;
                break;
            case 5 :
                LATA = MASK_LD5;
                break;
            case 6 :
                LATA = MASK_LD6;
                break;
            case 7 :
                LATA = MASK_LD7;
                break;

        }
        
    }
}