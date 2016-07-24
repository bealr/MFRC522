/* 
 * File:   SPI.c
 * Author: rbeal
 *
 * Created on July 16, 2016, 11:54 AM
 */

#include "SPI.h"
#include <xc.h>

void init_SPI() 
{
    TRISB0 = 1; // MISO
    TRISB1 = 0; // SCL
    TRISC7 = 0; // MOSI
    LATA5 = 1;  // CS
    
    SSPM0 = 0;  // Master mode : Fosc/64
    SSPM1 = 1;  // .
    SSPM2 = 0;  // .
    SSPM3 = 0;  // .
    
    SMP = 0;    // "Input data sampled at middle of data output time"
    CKE = 1;    // "Transmit occurs on transition from active to Idle clock state"
    
    SSPIE = 0;  // disable MSSP interrupt
    SSPIF = 0;  // clear MSSP interrupt
    SSPEN = 1;  // Enable MSSP (MSSP = SPI or I2C)
}

void transmit_SPI(char addr, char data)
{
    char dromadeuse;
    //LATB2 = 1;
    LATA5 = 0;              // CS
    
    SSPBUF = (addr<<1)&0x7E;
    while(!SSPSTATbits.BF); // wait while data not transmitted
    dromadeuse = SSPBUF;    // on a rien vu
    
    SSPBUF = data;          // send data
    while(!SSPSTATbits.BF);
    dromadeuse = SSPBUF;    // on a rien vu
    
    LATA5 = 1;              // CS
    //LATB2 = 0;
    
    //__delay_ms(5);        // for debugging
}

char receive_SPI(char addr)
{
    char tmp;
    char dromadeuse;
    //LATB2 = 1;
    LATA5 = 0;                      // CS
    
    SSPBUF = ((addr<<1)&0x7E)|0x80; // 0x80 = Read mode
    while(!SSPSTATbits.BF);         // wait while data not transmitted
    dromadeuse = SSPBUF;            // on a rien vu
    
    SSPBUF = 0x00;                  // required to generate clock
    while(!SSPSTATbits.BF);
    
    LATA5 = 1;                      // CS
    //LATB2 = 0;
    
    //__delay_ms(5);                // for debugging
    
    return SSPBUF;                  // slave response
}

void clear_bits_SPI(char addr, char mask)
{
    char tmp;
    
    tmp = receive_SPI(addr);
    transmit_SPI(addr, tmp&(~mask)); // change only masked bit
}

void set_bits_SPI(char addr, char mask)
{
    char tmp;
    
    tmp = receive_SPI(addr);
    transmit_SPI(addr, tmp|mask);   // change only masked bit
}