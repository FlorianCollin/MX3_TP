#include "config_bits.h"
#include "minimal_lcd.h"

////////////////////// MACROS ////////////////////////////
#define BASE_TIMER_FREQ 80000000
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

// CONFIG TIMER 2
#define MASK_CON_ON (1 << 15)
#define MASK_CON_TGATE_OFF (1 << 7)
#define MASK_CON_TCS (1 << 1) // TSC = 0 (internal clock)
#define MASK_CON_PRES 0
#define PERIODE 3600

// CONFIG OC1
#define MASK_OC_ON (1<<15)       // module ON (1)
#define MASK_OC_OCTSEL (1<<3)    // time2 selected (0)
#define MASK_OC_MODE 6           // PWM Mode 110 / 6


//// PROJET CONSTANTE //////////////

#define NB_SECONDE 60
#define FS_AUDIO 22050

///////////////////////////////////////////////////////////

///////////// SET FUNCTION /////////////////

void set_OC1() {

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

void set_timer2() {
    T2CON |= MASK_CON_ON; // module ON (1)
    T2CON &= ~MASK_CON_TCS; // Internal clock used (0)
    T2CON &= ~MASK_CON_TGATE_OFF; // Gate timer off (0)
    T2CON |= MASK_CON_PRES; // prescale 1:
    
    PR2 = PERIODE;
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
    // il faut brancher rf2 � spi1 output // 1000 = SDO1
    RPF2R = 0x8;
    SDI1R = 0xF;
    //////////////////////////////////////////////////////
    /////////////////////////////////////////////////////
    //SPI1BRG = 10;
    SPI1CON  = 0x8320;

    SPI1CON2 = 0;    
}

void seq_SPI() {
    ///////// S�quence FLASH ////////
    // SS = RF8
    LATF &= ~(1<<8); // SS = 0
    LCD_Init(1,1);
    // Ecriture de 0x9F sur la voir MOSI
    SPI1BUF = 0x9F;
    // il faut attendre l'envoie, on attend alors que busy passe � 0
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
    LATF |= (1<<8);
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
    /// LED ///
    set_led();
    LATA  &= ~(1);

    //// SPI ///
    init_SPI();
    seq_SPI();



    //LCD_Write_String("hello world");
    //unsigned char count = 0x0;

    /// LECTURE MEMOIRE FLASH ///
    int data;
    flash_read_init();
    for(int i = 0; i < (LAST_AD - START_AD); i++) {
        data = flash_read_to_current_position();
        DelayUs(10000);
        LCD_Clear();
        LCD_Write_HEX(data);
    }
    flash_read_end();
    
    while(1) {

    
    }
   
}