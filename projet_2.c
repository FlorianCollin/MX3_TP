#include "config_bits.h"
#include "minimal_lcd.h"

////////////////////// MACROS ////////////////////////////
#define BASE_TIMER_FREQ 10000000
#define SPI1BUSY (SPI1STAT & (1<<11))
#define START_AD 0x32 // 50
#define LAST_AD 0x143029 // 1323049

#define MASK_BTNC (1) // RF0

// LED (FOR DEBUG)
#define MASK_LD7 (1<<7)
#define MASK_LD6 (1<<6)
#define MASK_LD5 (1<<5)
#define MASK_LD4 (1<<4)
#define MASK_LD3 (1<<3)
#define MASK_LD2 (1<<2)
#define MASK_LD1 (1<<1)
#define MASK_LD0 (1)

// SW0 (FOR DEBUG)
#define MASK_SW0 (1 << 3)

// CONFIG TIMER 2/3
#define MASK_CON_ON (1 << 15)
#define MASK_CON_TGATE_OFF (1 << 7)
#define MASK_CON_TCS (1 << 1) // TSC = 0 (internal clock)
#define MASK_CON_PRES_TIMER_2 0
#define MASK_CON_PRES_TIMER_3  (1<<6) | (1<<5) | (1<<4)
#define PERIODE_TIMER2 453
#define PERIODE_TIMER3 39062
#define MASK_IFS0_TIMER2 (1 << (2*5 - 1))
#define MASK_IFS0_TIMER3 (1 << (3*5 - 1))


// CONFIG OC1
#define MASK_OC_ON (1<<15)       // module ON (1)
#define MASK_OC_OCTSEL (1<<3)    // time2 selected (0)
#define MASK_OC_MODE 6           // PWM Mode 110 / 6

// AUDIO OUT
#define MASK_AUDIO_OUT (1<<14)

//// PROJET CONSTANTE //////////////

#define NB_SECONDE 60
#define FS_AUDIO 22050

/////////////////////// VARIABLE GLOABLE ////////////////////////
// STATE FSM :
// 0 : Init / On ne fais rien, on attent l'appuie sur le boutton btnc
// 1 : On fais la commande de l'écture de la mémoire flash on passe directement à 2
// 2 : On joue la musique et on incrémente le compteur 1Hz de l'écran LCD
// 3 : On est en pausse on attend btnc
char state = 0;
///////////////////////////////////////////////////////////

///////////// SET FUNCTION /////////////////

void set_OC1CON() {
    OC1CON |= MASK_OC_ON; // (1)
    OC1CON &= ~MASK_OC_OCTSEL; // (0)
    OC1CON |=  MASK_OC_MODE; // 6

    OC1R = 0;
    OC1RS = PERIODE_TIMER2/2;

    // redirection OC1 to RB14

    RPB14R = 0xC;

}

void set_OC1RS(int k) {
    OC1RS = k;
}

void set_led() {
    // LED SET AS OUTPUT (0)
    TRISA &= ~MASK_LD0;
    TRISA &= ~MASK_LD1;
    TRISA &= ~MASK_LD2;
    TRISA &= ~MASK_LD3;
    TRISA &= ~MASK_LD4;
    TRISA &= ~MASK_LD5;
    TRISA &= ~MASK_LD6;
    TRISA &= ~MASK_LD7;
}

void set_sw0() {
    // SWO SET AS INPUT (1)
    TRISF |= MASK_SW0;
}

/// AUDIO OUTPUT SET //

void set_audio_output() {
    TRISB &= ~MASK_AUDIO_OUT;
    ANSELB &= ~MASK_AUDIO_OUT;
}

/// TIMER 2 ///

void set_timer2() {
    T2CON |= MASK_CON_ON; // module ON (1)
    T2CON &= ~MASK_CON_TCS; // Internal clock used (0)
    T2CON &= ~MASK_CON_TGATE_OFF; // Gate timer off (0)
    T2CON |= MASK_CON_PRES_TIMER_2; // prescale 1:
    
    PR2 = PERIODE_TIMER2;
}

/// TIMER 3 ///

void set_timer3() {
    T3CON |= MASK_CON_ON; // module ON (1)
    T3CON &= ~MASK_CON_TCS; // Internal clock used (0)
    T3CON &= ~MASK_CON_TGATE_OFF; // Gate timer off (0)
    T3CON |= MASK_CON_PRES_TIMER_3; // prescale 1:
    
    PR3 = PERIODE_TIMER3;
}

void set_interrupt() {
    INTCON |= (1 << 12); // (1)
    __asm__ ("ei");
}

//////////// GET STATE FUNCTION ////////////////
void set_btnc() {
    TRISF |= MASK_BTNC; // input
}

int is_btnc() {
    if ((PORTF & MASK_BTNC) == 0) {
        
        return 0;
    }
    return 1;
}

int is_sw0() {
    if ((PORTF & MASK_SW0) == 0) {
        return 0;
    }
    return 1;
}

//////////// SPI FUNCTION /////////////////////

void wait_SPI_transfer(){
    while(SPI1BUSY) {}
}

void init_SPI() {
    ////////// INPUT - OUTPUT //////////////////
    // output (0) //
    // SS
    TRISF &= ~(1<<8);
    LATF |= (1<<8);
    // SCLK
    // TRISF &= ~(1<<6);
    //MOSI
    //TRISF &= ~(1<<2);
    // input (1) //
    // MISO
    //TRISF |= (1<<7);
    
    /////////////////////////////////////////////
    
    // // Configuration SPI1CON
    // /////// SPI1CON ///////
    // SPI1CON &= ~(1<<31); // framed disabled (0)
    // SPI1CON &= ~(1<<23); // PBCLK (0)
    // SPI1CON |= (1<<5); // MSTEN (1)
    // SPI1CON |= (1<<15); // (1)
    // // 16 // 7 // 11 // 10
    
    // // CKE = 0 // CKP = 1
    // SPI1CON |= (1<<8);
    // SPI1CON &= ~(1<<6);
    
    // SPI1CON |= (1<<13);

    // /////// SPI1CON2 /////
    // SPI1CON2 &= ~(1<<7); // Audio protocole disabled (0)
    
    // // SPI1BRG = 0;
    
    ///////// branchement ///////////////////////////////
    // il faut brancher rf2 ? spi1 output // 1000 = SDO1
    RPF2R = 0x8;
    SDI1R = 0xF;
    //////////////////////////////////////////////////////
    /////////////////////////////////////////////////////
    //SPI1BRG = 10;
    SPI1CON  = 0x8320;

    SPI1CON2 = 0;    
}

void seq_SPI() {
    ///////// S?quence FLASH ////////
    // SS = RF8
    LATF &= ~(1<<8); // SS = 0
    DelayUs(10000);
    // Ecriture de 0x9F sur la voir MOSI
    SPI1BUF = 0x9F;
    // il faut attendre l'envoie, on attend alors que busy passe ? 0
    wait_SPI_transfer();
    int first_read = SPI1BUF;
    // Ecriture 
    SPI1BUF = 0x00;
    wait_SPI_transfer();
    // Lecture dans le registre de reception
    int valeur_lue = SPI1BUF;
    LCD_Write_HEX(valeur_lue);
    // On envoie encore une valeur inutile
    SPI1BUF = 0x00;
    wait_SPI_transfer();
    int fa = SPI1BUF;
    LCD_Write_HEX(fa);
    SPI1BUF = 0x00;
    wait_SPI_transfer();
    int mem = SPI1BUF;
    LCD_Write_HEX(mem);
    LATF |= (1<<8); // SS = 1
}

// LECTURE DE LA FLASH

void flash_read_init() {
    int tmp;
    LATF &= ~(1<<8); // SS = 0
    DelayUs(1000000);
    
    SPI1BUF = 0x03;             // commande : lecture de la memoire
    wait_SPI_transfer();        // attente de l'envoie
    tmp = SPI1BUF;              // lecture du buffer
    
    // Envoie des 3 octets d'adresse 0x000032
    
    SPI1BUF = 0x00;
    wait_SPI_transfer();
    tmp = SPI1BUF;
    
    SPI1BUF = 0x00;
    wait_SPI_transfer();
    tmp = SPI1BUF;
            
    SPI1BUF = 50;
    wait_SPI_transfer();
}

int flash_read_to_current_position() {
    int data = SPI1BUF;          // lecture
    SPI1BUF = 0x00;              // ecriture
    wait_SPI_transfer();         // attente
    return data;
}

void flash_read_end() {
    LATF |= (1<<8); // SS = 1
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////////////////////////////////////

void main() {
    /// DEBUG OUTPUT/INPUT SET ///
    set_led();
    set_sw0();
    ////////////////////////////
    set_btnc();
    set_timer2();
    set_timer3();
    set_audio_output();
    set_OC1CON();
    

    /// LCD ///
    LCD_Init(1,1);
    LCD_Clear();
    DelayUs(100000);
    LCD_Write_String("Hello");
    DelayUs(100000);
    LCD_Clear();
    
    //LCD_Clear();
    /// SPI ///
    init_SPI();
    DelayUs(100000);
    seq_SPI();
    LCD_Clear();

    // /// LECTURE MEMOIRE FLASH ///
    int data = 0;
    flash_read_init();
    int count = 0;
    
    int count_max = (LAST_AD - START_AD);
    
    // int state = 0; DEFINI EN VARIABLE GLOBAL
    int current_time = 0;
    int current_real_time = 0;
    int last_time_btnc = 0;
    int tmp = 1;
    while(count < count_max) {

        if (tmp == 1) {
            data = flash_read_to_current_position();
            data = (data+128)*PERIODE_TIMER2/256;
            tmp = 0;
        }

        if ((IFS0 & MASK_IFS0_TIMER2) && state == 1) {
            IFS0 &= ~MASK_IFS0_TIMER2; // remise à zéro du flag
            set_OC1RS(data);
            count++;  
            tmp = 1;
        }

        if (is_btnc()) {
            //if ((current_real_time - last_time_btnc) < 2){ // On doit avoir au moins 2 seconde qui sépare deux appuie de bouton
                //last_time_btnc = current_real_time;
                if (state == 0) {
                    state = 1;
                } else if (state == 2) {
                    state = 3;
                } else if (state == 3) {
                    state == 0;
                    current_time = 0;
                }
            //}
                
        }

        if (state == 1) {
            // On redémare à zéro
            flash_read_end(); // SS 1
            flash_read_init();
            state = 2;
            count = 0;
        } else if (state == 0) {
           
        }

        if ((IFS0 & MASK_IFS0_TIMER3)){
            current_real_time++;
            if (state == 2) {
                current_time++;
            }
            LCD_Clear();
            LCD_Write_HEX(current_time);
        }
       
    
    }
    flash_read_end();
   
}