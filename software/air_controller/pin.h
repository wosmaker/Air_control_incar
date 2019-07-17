 #pragma once

//  Arduino pin assignment
// 
//                   ATMega328p
//                    +-\/-+
//              PC6  1|    |28  PC5 (AI 5 / D19)
//        (D 0) PD0  2|    |27  PC4 (AI 4 / D18)
//        (D 1) PD1  3|    |26  PC3 (AI 3 / D17)
//        (D 2) PD2  4|    |25  PC2 (AI 2 / D16)
//   PWM+ (D 3) PD3  5|    |24  PC1 (AI 1 / D15)
//        (D 4) PD4  6|    |23  PC0 (AI 0 / D14)
//              VCC  7|    |22  GND
//              GND  8|    |21  AREF
//              PB6  9|    |20  AVCC
//              PB7 10|    |19  PB5 (D 13)
//   PWM+ (D 5) PD5 11|    |18  PB4 (D 12)
//   PWM+ (D 6) PD6 12|    |17  PB3 (D 11) PWM
//        (D 7) PD7 13|    |16  PB2 (D 10) PWM
//        (D 8) PB0 14|    |15  PB1 (D 9)  PWM
//                    +----+


// fan control **pwm with low frequency
#define _front_fan 				10	
#define _rear_fan					9

// left encoder **leften -> left encoder
#define _leften_outa			17
#define _leften_outb			18
#define _leften_btn				19

// right encoder **righten -> right encoder
#define _righten_outa			2
#define _righten_outb			3
#define _righten_btn			4	

// compressor control
#define _compressor				5

// lcd pin (spi)
#define tft_blacklight		6
#define tft_sck						13
#define tft_sda						11
#define tft_dc						8
#define tft_cs 						12
#define tft_reset					7

// sensor analog pin
#define _front_temp				A2
#define _rear_temp				A1

