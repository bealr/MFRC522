/*
 * File:   IR.c
 * Author: rbeal
 *
 * Created on Febuary 02, 2016, 18:52 PM
 */

#include "uart.h"
#include <xc.h>

void init_soft_uart()
{
    TMR0ON = 1;
    T0CS = 0;
    PSA = 1;
    TRISC6 = 0;
    LATC6 = 1;
}

void delay_100u()
{
    TMR0L = 255-155;
    TMR0IF = 0;
    while(!TMR0IF);
}

void uart_write_c(char data)
{
    int i;
    
    LATC6 = 0; // start bit
    delay_100u();
    
    
    for (i=0;i<8;i++)
    {
        LATC6 = (data >> i)&0x01;
        delay_100u();
    }
    
    LATC6 = 1;   // stop bit
    delay_100u();
    
    __delay_ms(5);

}

void uart_write_s(const char *data)
{
    int i;
    
    for (i=0;*(data+i) != '\0';i++)
    {
        uart_write_c(*(data+i));
    }
}