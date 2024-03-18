#include "config_bits.h"
#include "minimal_lcd.h"

////////////////////// MACROS ////////////////////////////
#define BASE_TIMER_FREQ 80000000
#define SPI1BUSY (SPI1STAT & (1<<11))
#define START_AD 0x32 // 50
#define LAST_AD 0x143029 // 1323049
///////////////////////////////////////////////////////////

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
    // il faut brancher rf2 à spi1 output // 1000 = SDO1
    RPF2R = 0x8;
    SDI1R = 0xF;
    //////////////////////////////////////////////////////
    /////////////////////////////////////////////////////
    //SPI1BRG = 10;
    SPI1CON  = 0x8320;

    SPI1CON2 = 0;    
}

void seq_SPI() {
    ///////// Séquence FLASH ////////
    // SS = RF8
    LATF &= ~(1<<8); // SS = 0
    LCD_Init(1,1);
    // Ecriture de 0x9F sur la voir MOSI
    SPI1BUF = 0x9F;
    // il faut attendre l'envoie, on attend alors que busy passe à 0
    wait_SPI_transfer();
    int first_read = SPI1BUF;
    // Ecriture 
    SPI1BUF = 0x00;
    wait_SPI_transfer();
    // Lecture dans le registre de réception
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

void flash_read_init() {
    int tmp;
    LATF &= ~(1<<8); // SS = 0
    SPI1BUF = 0x03;
    wait_SPI_transfer();
    //tmp = SPI1BUF;
    SPI1BUF = 0x00;
    wait_SPI_transfer();
    //tmp = SPI1BUF;
    SPI1BUF = 0x00;
    wait_SPI_transfer();
    //tmp = SPI1BUF;
    SPI1BUF = START_AD;
}
int flash_read_to_current_position() {
    int data = SPI1BUF;
    SPI1BUF = 0x00;
    wait_SPI_transfer();
    return data;
}

void flash_read_end() {
    LATF |= (1<<8); // SS = 1
}

void main() {
    // LED 0 high
    TRISA &= ~(1);
    LATA  &= ~(1);

    //////////////////
    init_SPI();
    seq_SPI();
    //LCD_Write_String("hello world");
    //unsigned char count = 0x0;
    flash_read_init();
    int count = 0x0;
    int data;
    while(count = LAST_AD - START_AD) {
        // boucle principale
        // count = (count + 1) & 0x00FF;
        // DelayUs(10000000);
        // LCD_Set_Cursor_Pos(1,15-1);
        // LCD_Write_HEX(count);
        
        data = flash_read_to_current_position();
        DelayUs(100000);
        LCD_Clear();
        LCD_Write_HEX(data);
        count ++;
    }
    flash_read_end();
}