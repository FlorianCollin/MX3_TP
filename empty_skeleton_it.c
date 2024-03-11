#include "config_bits.h"



void __attribute__((interrupt(ipl1soft), vector(_TIMER_1_VECTOR))) Timer1Handler ( void ) {

}

void __attribute__((interrupt(ipl1soft), vector(_TIMER_2_VECTOR))) Timer2Handler ( void ) {

}


void main() {
    // Peripheral Initialization
    // .....
    // .....
    
    // interrupt generic parameters
    INTCON = ...;
    
    // set interrupt priority
    IPCxxx = ...;
    ...
    
    // select which interrupts are allowed
    IECxxx = ...;
    ...     

    // globally enable allowed interrupts
    __asm__ ("ei");
    
    
    // loop
    while(1)
    {
 
        
    }
}
