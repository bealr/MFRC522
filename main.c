/*
 * File:   main.c
 * Author: rbeal
 *
 * Created on July 16, 2016, 11:03 AM
 */


// PIC18F2550 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1L
#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = INTOSCIO_EC// Oscillator Selection bits (Internal oscillator, port function on RA6, EC used by USB (INTIO))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)


// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 0         // Brown-out Reset Voltage bits (Maximum setting 4.59V)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = OFF     // CCP2 MUX bit (CCP2 input/output is multiplexed with RB3)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = OFF     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "SPI.h"
#include "RC522.h"
#include "uart.h"


void init();

void main(void) {
    
    int i;
    char status;
    char str[100];
    
    init(); // initialize PIC
    init_soft_uart();
    init_SPI(); 
    
    
    GIE = 0; // disable all PIC's interrupts 
    
    LATB2 = 0; // LED0 off
    for (i=0;i<100;i++) __delay_ms(10);
    
    uart_write_s("init ok !\n\r");
    
    init_RC522();
        
    while (1)
    {
        status = MFRC522_Request(PICC_REQIDL, str); // check if card is present
        if (status == MI_OK)
        {
            uart_write_s("ok card ! \n\r");
            
            if (MFRC522_ReadCardSerial(str) == MI_OK) // get UID
            {
                uart_write_s("UID = ");     // and display it
                uart_write_hex_c(*(str+0));
                uart_write_hex_c(*(str+1));
                uart_write_hex_c(*(str+2));
                uart_write_hex_c(*(str+3));
                uart_write_s("\n\r");
            }
            
            LATB2 = 1;                         // blink LED0
            for (i=0;i<10;i++) __delay_ms(10); // .
            LATB2 = 0;                         // .
            for (i=0;i<10;i++) __delay_ms(10); // .
            LATB2 = 1;                         // .
            for (i=0;i<10;i++) __delay_ms(10); // .
            LATB2 = 0;                         // .
            for (i=0;i<10;i++) __delay_ms(10); // .
            LATB2 = 1;                         // .
            for (i=0;i<10;i++) __delay_ms(10); // .
            LATB2 = 0;                         // .
            for (i=0;i<10;i++) __delay_ms(10); // .
        }
        
        //for (i=0;i<50;i++) __delay_ms(10);
        LATB2 = 1;      // Blink LED0, visual normal operation works, before watchdog dev
        __delay_ms(10);
        LATB2 = 0;
        __delay_ms(10);
    }
    
    return;
}

void init()
{
    // OSCILLATOR SETUP
    OSCCON = 0b01110100; // internal osc: 8MHz / frequency stable
    
    //GPIO
    TRISB2 = 0; // LED0
    TRISA0 = 0; // RESET MFRC522
    TRISA5 = 0; // CS MFRC522
    
}