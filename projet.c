#include "config_bits.h"
#include "minimal_lcd.h"

#define BASE_TIMER_FREQ 80000000

#define SPI1BUSY (SPI1STAT & (1<<11))


void wait_SPI_transfer(){
    while(SPI1BUSY) {}
    //DelayUs(100000000);
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
    // il faut brancher rf2 à spi1 output // 1000 = SDO1
    RPF2R = 0x8;
    SDI1R = 0xF;
    //////////////////////////////////////////////////////
    /////////////////////////////////////////////////////
    SPI1BRG = 0;
    SPI1CON  = 0x8320;
    SPI1CON2 = 0;
    

    
}

void main() {
    // LED 0 high
    TRISA &= ~(1);
    LATA  &= ~(1);

    //////////////////


    init_SPI();

    

    
    //LCD_Write_String("hello world");
    //unsigned char count = 0x0;



    ///////// Séquence FLASH ////////
    // SS = RF8
    LATF &= ~(1<<8); // SS = 0
    LCD_Init(1,1);
    // Ecriture de 0x9F sur la voir MOSI
    SPI1BUF = 0x9F;
    // il faut attendre l'envoir, on attend alors que busy passe à 0
    wait_SPI_transfer();
    SPI1BUF = 0x69;
    wait_SPI_transfer();
    // Lecture dans le registre de réception
    int valeur_lue = SPI1BUF;
    LCD_Write_HEX(valeur_lue);
    // On envoie encore une valeur inutile
    SPI1BUF = 0x69;
    wait_SPI_transfer();
    int fa = SPI1BUF;
    LCD_Write_HEX(fa);
    SPI1BUF = 0x69;
    wait_SPI_transfer();
    int mem = SPI1BUF;
    LCD_Write_HEX(mem);
    LATF |= (1<<8);

    LATA &= ~1; // LED0 low







    
    while(1) {
        // boucle principale
        // count = (count + 1) & 0x00FF;
        // DelayUs(10000000);
        // LCD_Set_Cursor_Pos(1,15-1);
        // LCD_Write_HEX(count);
        



    }
}