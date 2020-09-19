/*
 * File:   PIC16F877a_I2C.c
 * Author: Hasan
 * IDE: MPLABX by MicroChip
 * Created on September 17, 2020, 7:26 PM
 */

//Configuration Bits
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define _XTAL_FREQ 20000000 

#include <xc.h>

void I2C_Initialize(const unsigned long feq_K) //Begin IIC as master
{
  TRISCbits.TRISC3 = 1; //Set SCL pin as input pins
  TRISCbits.TRISC4 = 1;  //Set SDA pins as input pins
  
  SSPCON  = 0b00101000;    //pg84/234 
  SSPCON2 = 0b00000000;    //pg85/234
  
  SSPADD = (_XTAL_FREQ/(4*feq_K*100))-1; //Setting Clock Speed pg99/234
  SSPSTAT = 0b00000000;    //pg83/234
}

void I2C_Hold()
{
    while (   (SSPCON2 & 0b00011111)    ||    (SSPSTAT & 0b00000100)   ) ; //check the bis on registers to make sure the IIC is not in progress
}

void I2C_Begin()
{
  I2C_Hold();  //Hold the program is I2C is busy  
  SEN = 1;     //Begin IIC pg85/234
}

void I2C_End()
{
  I2C_Hold(); //Hold the program is I2C is busy  
  PEN = 1;    //End IIC pg85/234
}

void I2C_Write(unsigned data)
{
  I2C_Hold(); //Hold the program is I2C is busy 
  SSPBUF = data;         //pg82/234
}

unsigned short I2C_Read(unsigned short ack)
{
  unsigned short incoming;
  I2C_Hold();
  RCEN = 1;
  
  I2C_Hold();
  incoming = SSPBUF;      //get the data saved in SSPBUF
  
  I2C_Hold();
  ACKDT = (ack)?0:1;    //check if ack bit received  
  ACKEN = 1;          //pg 85/234
  
  return incoming;
}

void main(void) {
    I2C_Initialize(100);
    
    while(1){
        I2C_Begin();
        I2C_Write(0x9F);
        I2C_Write(0xAD);
        I2C_Write(0x7A);
        I2C_End();
        __delay_ms(1000);
        
    }
            
    return;
}
