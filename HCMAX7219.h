/* FILE:    HCMAX7219.cpp
   DATE:    11/03/15
   VERSION: 0.1
   AUTHOR:  Andrew Davies

Library for Maxim MAX7219 LED driver IC.

You may copy, alter and reuse this code in any way you like, but please leave
reference to HobbyComponents.com in your comments if you redistribute this code.
This software may not be used directly for the purpose of selling products that
directly compete with Hobby Components Ltd's own range of products.

THIS SOFTWARE IS PROVIDED "AS IS". HOBBY COMPONENTS MAKES NO WARRANTIES, WHETHER
EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, ACCURACY OR LACK OF NEGLIGENCE.
HOBBY COMPONENTS SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR ANY DAMAGES,
INCLUDING, BUT NOT LIMITED TO, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY
REASON WHATSOEVER.
*/

#ifndef HCMAX7219_h
#define HCMAX7219_h
#include "Arduino.h"

/***************************** USER CONFIGURATION ****************************/

/* CHANGE THIS VALUE IF YOU ARE DASY CHAINING MORE THAN ONE DRIVER */
#define NUMBEROFDRIVERS 1

/* RESERVED FOR FUTURE USE. PLEASE DO NOT ALTER */
#define SEVENSEGMENT

/*****************************************************************************/


/* Defines the size of the output buffer */
#define DISPLAYBUFFERSIZE NUMBEROFDRIVERS * 8

/* MAX7219 register addresses */
#define MAX7219NOOP 0x00
#define MAX7219DIGIT1 0x01
#define MAX7219DIGIT2 0x02
#define MAX7219DIGIT3 0x03
#define MAX7219DIGIT4 0x04
#define MAX7219DIGIT5 0x05
#define MAX7219DIGIT6 0x06
#define MAX7219DIGIT7 0x07
#define MAX7219DIGIT8 0x08
#define MAX7219DECODE 0x09
#define MAX7219INTESITY 0x0A
#define MAX7219SCANLIMIT 0x0B
#define MAX7219SHUTDOWN 0x0C
#define MAX7219TEST 0x0F

/* MAX7219 register options */
#define TESTMODEOFF 0
#define TESTMODEON 1
#define MAX7219OFF 0
#define MAX7219ON 1

/* Character map for seven-segment display */
#ifdef SEVENSEGMENT
const byte SevenSegChar[101] = {0x00, // SPACE
                               0xA0, // !
                               0x22, // "
                               0x36, // #
                               0x4B, // $
                               0x5A, // %
                               0x6F, // &
                               0x02, // '
                               0x4E, // (
                               0x78, // )
                               0x63, // *
                               0x07, // +
                               0x18, // ,
                               0x01, // -
                               0x80, // .
                               0x25, // / 
                               0x7E, // 0
                               0x30, // 1
                               0x6D, // 2
                               0x79, // 3
                               0x33, // 4
                               0x5B, // 5
                               0x5F, // 6
                               0x70, // 7
                               0x7F, // 8        
                               0x7B, // 9
                               0x09, // :
                               0x19, // ;
                               0x43, // <
                               0x41, // =
                               0x61, // >
                               0x65, // ?
                               0x7D, // @
                               0x77, // A
                               0x1F, // B
                               0x4E, // C
                               0x3D, // D
                               0x4F, // E
                               0x47, // F
                               0x5E, // G
                               0x37, // H
                               0x30, // I
                               0x38, // J
                               0x57, // K
                               0x0E, // L
                               0x54, // M
                               0x76, // N
                               0x7E, // O
                               0x67, // P
                               0x73, // Q
                               0x66, // R
                               0x5B, // S
                               0x0F, // T
                               0x3E, // U
                               0x3A, // V
                               0x2A, // W
                               0x37, // X
                               0x3B, // Y
                               0x69, // Z
                               0x4E, // [
                               0x13, // |
                               0x78, // ]
                               0x62, // ^
                               0x08, // _ 
                               0x02, // '      
                               0x77, // a
                               0x1F, // b
                               0x4E, // c
                               0x3D, // d
                               0x4F, // e
                               0x47, // f
                               0x5E, // g
                               0x37, // h
                               0x30, // i
                               0x38, // j
                               0x57, // k
                               0x0E, // l
                               0x54, // m
                               0x76, // n
                               0x7E, // o
                               0x67, // p
                               0x73, // q
                               0x66, // r
                               0x5B, // s
                               0x0F, // t
                               0x3E, // u
                               0x3A, // v
                               0x2A, // w
                               0x37, // x
                               0x3B, // y
                               0x69}; // z                          
#endif


class HCMAX7219
{
  public:
  HCMAX7219(byte DIN, byte CLK, byte LOAD);
  void Write(byte Address, byte Data, byte Driver);
  void Shutdown(boolean Mode, byte Driver);
  void TestMode(boolean Mode, byte Driver);
  void SevenSegDigits(byte Digits, byte Driver);
  void Intensity(byte Level, byte Driver);
  void Clear(void);
  void Refresh(void);

#ifdef SEVENSEGMENT
  void print7Seg(char TextString[], unsigned int Offset);
  void print7Seg(long number, unsigned int Offset);
  void print7Seg(long number, byte decimalPlace, unsigned int Offset);
#endif  
 
  byte DisplayBuffer[DISPLAYBUFFERSIZE];
  
  private:
  void Init(void);
  byte _DIN;
  byte _CLK; 
  byte _LOAD;
};  


#endif