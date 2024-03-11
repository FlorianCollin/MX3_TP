#include "config_bits.h"

#define MASK_LD0 (1)
#define PERIODE 19531
#define TIMER_NUMBER 2
#define MASK_IFS0 (1 << (TIMER_NUMBER*5 - 1))

// CONFIG TIMER 2
#define MASK_CON_ON (1 << 15)
#define MASK_CON_TGATE_OFF (1 << 7)
#define MASK_CON_TCS (1 << 1) // TSC = 0 (internal clock)
#define MASK_CON_PRES (1<<6) | (1<<5) | (1<<4)

// LED RGB
#define MASK_LEDR (1<<2)
#define MASK_LEDG (1<<12)
#define MASK_LEDB (1<<3)

//OC3CON
#define MASK_OC3CON_ON (1<<15) // module ON (1)
#define MASK_OC3CON_OCTSEL (1<<3) // time2 selected (0)
#define MASK_OC3CON_MODE (1<<2) // single pulse (100)

// Fonctionne !
void wait_IFS0_MODE() {
    IFS0 = IFS0 & ~MASK_IFS0 ;
    // remise à zéro du bit 
    while(1) {
        if ((IFS0 & MASK_IFS0) != 0){
                // Une periode à déja était fini
                IFS0 = IFS0 & ~MASK_IFS0 ;
                // remise à zéro du bit 
                
                return;
        }
    }
}

void main() {
    // configuration du timer2
    T2CON |= MASK_CON_ON; // module ON (1)
    T2CON &= ~MASK_CON_TCS; // Internal clock used (0)
    T2CON &= ~MASK_CON_TGATE_OFF; // Gate timer off (0)
    T2CON |= MASK_CON_PRES; // prescale 1:256
    
    
    // LED en output
    TRISA &= ~MASK_LD0; //(0)
    
    // LED RGB en output // (0)
    TRISD &= ~MASK_LEDR;
    TRISD &= ~MASK_LEDG;
    TRISD &= ~MASK_LEDB;
    
    PR2 = PERIODE;
    
    // OC3
    OC3CON |= MASK_OC3CON_ON; // (1)
    OC3CON &= ~MASK_OC3CON_OCTSEL; // (0)
    // mode (100)
    OC3CON |=  MASK_OC3CON_MODE;
    OC3CON &= ~ (1);
    OC3CON &= ~ (1<<1);
    
    // Réglage de la valeur de comparaison dans OC3
    OC3R = 0; // On compare la valeur du timer à 0
    
    RPD2R = 11;
    
    // EXO1
    /*
    while(1) {
        LATA = LATA & ~MASK_LD0 ; // LED0 OFF
        wait_IFS0_MODE(); // attente de 0.5 s      
        LATA = LATA | MASK_LD0 ; // LED0 ON
        wait_IFS0_MODE();

    }
    */
    
    while(1) {
        
    }
    
    

}