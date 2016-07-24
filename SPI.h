/* 
 * File:   SPI.h
 * Author: rbeal
 *
 * Created on July 16, 2016, 11:55 AM
 */

void init_SPI();

void transmit_SPI(char addr, char data);
char receive_SPI(char addr);

void clear_bits_SPI(char addr, char data);
void set_bits_SPI(char addr, char data);
