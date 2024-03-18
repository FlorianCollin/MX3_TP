#include "config_bits.h"
#include "minimal_lcd.h"

#define BASE_TIMER_FREQ 120000000

#define SPI1BUSY (SPI1STAT & (1<<11))


void main() {

    TRISA &= ~(1);
    // Configuration SPI1CON
    //enable :
    SPI1CON |= (1<<15);
    SPI1CON &= ~(1<<31); // framed disabled
    SPI1CON &= ~(1<<23); // PBCLK
    SPI1CON |= (1<<5); // MSTEN
    
    SPI1CON2 &= ~(1<<7);
    // CKE = 1 // CKP = 0
    SPI1CON |= (1<<8);
    SPI1CON &= ~(1<<6);

    //test

    SPI1BRG = 1;

    //

    // output (0) //
    // SS
    TRISF &= ~(1<<8);
    // SCLK
    TRISF &= ~(1<<6);
    //MOSI
    TRISF &= ~(1<<2);
    // il faut brancher rf2 à spi1 output // 1000 = SDO1
    RPF2R = (1<<3);

    // input (1) //
    // MISO
    TRISF |= (1<<7); // Audio protocol is disabled
    // branchement
    SDI1R = 0xF;
    
    




    LCD_Init(1,1);
    //LCD_Write_String("hello world");
    unsigned char count = 0x0;



    ///////// Séquence FLASH ////////
    // SS = RF8
    LATA ^= 1;
    LATF &= ~(1<<8); // SS = 0
    // Ecriture de 0x9F sur la voir MOSI
    SPI1BUF = 0x9F;
    // il faut attendre l'envoir, on attend alors que busy passe à 0
    while(!SPI1BUSY) {}

    SPI1BUF = 0x69;
    while(!SPI1BUSY) {}
    // Lecture dans le registre de réception
    int valeur_lue = SPI1BUF;
    LCD_Write_HEX(valeur_lue);
    // On envoie encore une valeur inutile
    SPI1BUF = 0x69;
    while(SPI1BUSY) {}
    int fa = SPI1BUF;
    LCD_Write_HEX(fa);
    SPI1BUF = 0x69;
    while(SPI1BUSY) {}
    int mem = SPI1BUF;
    LCD_Write_HEX(mem);

    LATF |= (1<<8);

    LATA ^= 1;







    
    while(1) {
        // boucle principale
        // count = (count + 1) & 0x00FF;
        // DelayUs(10000000);
        // LCD_Set_Cursor_Pos(1,15-1);
        // LCD_Write_HEX(count);
        



    }
}