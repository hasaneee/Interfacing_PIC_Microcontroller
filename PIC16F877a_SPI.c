/*
 * File:   PIC16F877a_SPI.c
 * Author: Hasan
 * IDE: MPLABX by MicroChip
 * Created on September 22, 2020, 5:22 PM
 */

// Configuration bits
#pragma config FOSC = XT   // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF  // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF   // Low-Voltage In-Circuit Serial Programming Enable bit
#pragma config CPD = OFF   // Data EEPROM Memory Code Protection bit 
#pragma config WRT = OFF   // Flash Program Memory Write Enable bits 
#pragma config CP = OFF    // Flash Program Memory Code Protection bit

#define _XTAL_FREQ 20000000
#include <xc.h>

void SPI_Initialize_Master()
{
    TRISC5 = 0; //
    SSPSTAT = 0b00000000; //pg 74/234
    SSPCON = 0b00100000; //pg 75/234
    TRISC3 = 0; //Set as output for slave mode 
}

void SPI_Initialize_Slave()
{
    TRISC5 = 0; // SDO pin should be declared as output 
    SSPSTAT = 0b00000000; //pg 74/234
    SSPCON = 0b00100000; //pg 75/234
    TRISC3 = 1; //Set as in out for master mode
}


void SPI_Write(char incoming) 
{
    SSPBUF = incoming; //Write the user given data into buffer
}

unsigned SPI_Ready2Read()
{
    if (SSPSTAT & 0b00000001)
        return 1;
    else
        return 0;
}

char SPI_Read() //Read the received data
{
    while ( !SSPSTATbits.BF ); // Hold till BF bit is set, to make sure the complete data is read
    return(SSPBUF); // return the read data
}

void main(void) {
    SPI_Initialize_Master();
    while(1){
        SPI_Write(0x0F);
        __delay_ms(1000);
        SPI_Write(0x0A);
        __delay_ms(100);
        SPI_Write(0x12);
        __delay_ms(100);
    }
    
    return;
}
