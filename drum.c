//  ???????
//  ????????????
//      JA1YTS:Toshiba Amature Radio Station
//      JK1MLY:Hidekazu Inaba
//  

// Washing machine for PIC12F1501
//  (C)2018 JA1YTS,JK1MLY All rights reserved.
// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
// 1. Redistributions of source code must retain the above copyright notice, 
// this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice, 
// this list of conditions and the following disclaimer in the documentation and/or 
// other materials provided with the distribution.

#define _XTAL_FREQ          8000000

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin) 
//#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on GP4/OSC2/CLKOUT pin, I/O function on GP5/OSC1/CLKIN)
//#pragma config FOSC = ECH       // Oscillator Selection (ECH, External Clock, High Power Mode (4-32 MHz): device clock supplied to CLKIN pin)
//#pragma config FOSC = HS        // Oscillator Selection (ECH, External Clock, High Power Mode (4-32 MHz): device clock supplied to CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT enabled)
//#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config PWRTE = ON      // Power-up Timer Enable bit (PWRT disabled)
//#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR) 
#pragma config MCLRE = OFF      // MCLR Pin Function Select bit (MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
//#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
//#pragma config IOSCFS = 4MHZ    // Internal Oscillator Frequency Select (4 MHz)
#pragma config BOREN = OFF       // Brown-out Reset Selection bits (BOR enabled)
//#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
//#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
//#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
//#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2 
#pragma config WRT = OFF          // Flash Memory Self-Write Protection (Write protection off)
//#pragma config PLLEN = ON       // PLL Enable (4x PLL enabled)
//#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
//#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
//#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled) 
#pragma config LVP = OFF         // Low-Voltage Programming Enable (Low-voltage programming enabled)

// Pin1 V+
// Pin2 RA5 PNP2
// Pin3 RA4 NPN2
// Pin4 RA3 N.C.
// Pin5 RA2 SW/LED
// Pin6 RA1 NPN1
// Pin7 RA0 PNP1
// Pin8 V-

#define SW_ON 100

// ---------- ---------- ---------- ----------

// Wait and SW check
int LP_SW(int tim)
{
    for(int lp=0; lp<tim; lp++){
        if((PORTA & 0x04) == 0){
            return(lp + 1);
        }
        __delay_ms(10);       
    }
    return(0);
}

// Right Turn High - Low
void R_ON(void)
{
//             76543210
    TRISA  = 0b00111111;    //INPUT  1
    NOP();
    PORTA  = 0b00110000;    //LEVEL
    NOP();
    TRISA  = 0b00111101;    //INPUT  1
    NOP();
    TRISA  = 0b00011101;    //INPUT  1
    NOP();
    TRISA  = 0b00011100;    //INPUT  1
    NOP();
    TRISA  = 0b00001100;    //INPUT  1
}

void R_OFF(void)
{
//             76543210
    TRISA  = 0b00011100;    //INPUT  1
    __delay_ms(100);
//    NOP();
    TRISA  = 0b00011101;    //INPUT  1
    NOP();
    TRISA  = 0b00111101;    //INPUT  1
    NOP();
    TRISA  = 0b00111111;    //INPUT  1
    NOP();
    PORTA  = 0b00100001;    //LEVEL
}

// Left Turn High - Low
void L_ON(void)
{
//             76543210
    TRISA  = 0b00111111;    //INPUT  1
    NOP();
    PORTA  = 0b00000011;    //LEVEL
    NOP();
    TRISA  = 0b00111110;    //INPUT  1
    NOP();
    TRISA  = 0b00101110;    //INPUT  1
    NOP();
    TRISA  = 0b00101100;    //INPUT  1
    NOP();
    TRISA  = 0b00001100;    //INPUT  1
}

void L_OFF(void)
{
//             76543210
    TRISA  = 0b00101100;    //INPUT  1
    __delay_ms(100);
//    NOP();
    TRISA  = 0b00101110;    //INPUT  1
    NOP();
    TRISA  = 0b00111110;    //INPUT  1
    NOP();
    TRISA  = 0b00111111;    //INPUT  1
    NOP();
    PORTA  = 0b00100001;    //LEVEL
}

// Brake Low - Low
void B_ON(void)
{
//             76543210
    TRISA  = 0b00111111;    //INPUT  1
    NOP();
    PORTA  = 0b00110011;    //LEVEL
    NOP();
    TRISA  = 0b00111110;    //INPUT  1
    NOP();
    TRISA  = 0b00101110;    //INPUT  1
    NOP();
    TRISA  = 0b00101100;    //INPUT  1
    NOP();
    TRISA  = 0b00001100;    //INPUT  1
}

void B_OFF(void)
{
//             76543210
    TRISA  = 0b00101100;    //INPUT  1
    NOP();
    TRISA  = 0b00101110;    //INPUT  1
    NOP();
    TRISA  = 0b00111110;    //INPUT  1
    NOP();
    TRISA  = 0b00111111;    //INPUT  1
    NOP();
    PORTA  = 0b00100001;    //LEVEL
}

// High Speed Free Z - Z
void HS_OFF(void)
{
//             76543210
    TRISA  = 0b00111111;    //INPUT  1
    NOP();
    PORTA  = 0b00100001;    //LEVEL
}

void blink(int num)
{
// LED RA2
    TRISA  = 0b00111111;    //INPUT  1
    NOP();
    PORTA  = 0b00100101;    //LEVEL
    TRISA  = 0b00111011;    //INPUT  1

    for(int i = 0; i < num; i++){
        __delay_ms(100);
        PORTA  = 0b00100001;    //LEVEL
        __delay_ms(100);
        PORTA  = 0b00100101;    //LEVEL
    }
    TRISA  = 0b00111111;    //INPUT  1
}

void check(void)
{
    blink(1);
    R_ON();
    __delay_ms(1000);
    R_OFF();
    __delay_ms(1000);

    blink(1);
    L_ON();
    __delay_ms(1000);
    L_OFF();
    __delay_ms(3000);

    blink(1);
    R_ON();
    __delay_ms(1000);
    R_OFF();
    B_ON();
    __delay_ms(1000);
    B_OFF();

    blink(1);
    L_ON();
    __delay_ms(1000);
    L_OFF();
    B_ON();
    __delay_ms(500);
    B_OFF();
    __delay_ms(2500);
}

int beat(void) {
  for(int j = 0; j < 3; j++){

    blink(1);
    for(int i = 0; i < 3; i++){
        R_ON();
        if(LP_SW(70) >0){
            R_OFF();
            return (SW_ON);
        }

        R_OFF();
        if(LP_SW(70) >0){
            return (SW_ON);
        }
    }

    blink(1);
    for(int i = 0; i < 3; i++){
        L_ON();
        if(LP_SW(70) >0){
            L_OFF();
            return (SW_ON);
        }

        L_OFF();
        if(LP_SW(70) >0){
            return (SW_ON);
        }
    }
  }
    return (EXIT_SUCCESS);
}

int turn(void)
{
    for(int i = 0; i < 2; i++){
        blink(1);
        R_ON();
        if(LP_SW(500) >0){
            R_OFF();
            return (SW_ON);
        }

        R_OFF();
        if(LP_SW(100) >0){
            return (SW_ON);
        }

        blink(1);
        L_ON();
        if(LP_SW(500) >0){
            L_OFF();
            return (SW_ON);
        }

        L_OFF();
        if(LP_SW(100) >0){
            return (SW_ON);
        }
    }
        
    return (EXIT_SUCCESS);
}

int push(void)
{
    for(int i = 0; i < 10; i++){
        blink(1);
        R_ON();
        if(LP_SW(30) >0){
            R_OFF();
            return (SW_ON);
        }

        R_OFF();
        if(LP_SW(70) >0){
            return (SW_ON);
        }

        L_ON();
        if(LP_SW(30) >0){
            L_OFF();
            return (SW_ON);
        }

        L_OFF();
        if(LP_SW(70) >0){
            return (SW_ON);
        }
    }

    return (EXIT_SUCCESS);
}

/*
 * 
 */
int main(int argc, char** argv) {
    unsigned char state = 0;  
 
    
//             76543210
    ANSELA = 0b00000000;    //ANALOG 1
    TRISA  = 0b00111111;    //INPUT  1
    PORTA  = 0b00100001;    //LEVEL
    OSCCON = 0b01110000;    //INT8M
    WPUA   = 0b00000100;    //PullUp 1
    nWPUEN = 0;
//  INTCON = 0b00000000;    //
//  ADCON0 = 0b00000000;    //
//  ADCON1 = 0b00000000;    //
//  ADCON2 = 0b00000000;    //

    HS_OFF();
    
    blink(5);
    __delay_ms(5000);

//    check();
//    __delay_ms(2000);

    while(1){

        blink(2);
        __delay_ms(1000);
        if(beat() != SW_ON){
            LP_SW(500);
        }
        __delay_ms(100);

        blink(3);
        __delay_ms(1000);
        if(turn() != SW_ON){
            LP_SW(500);
        }
        __delay_ms(100);

        blink(4);
        __delay_ms(1000);
        if(push() != SW_ON){
            LP_SW(500);
        }
        __delay_ms(100);
        
    }

    return (EXIT_SUCCESS);
}

