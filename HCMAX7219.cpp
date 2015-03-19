/* FILE:    HCMAX7219.cpp
   DATE:    19/03/15
   VERSION: 0.2
   AUTHOR:  Andrew Davies

11/03/15 version 0.1: Original version

19/03/15 version 0.2: Added support for serial dot matrix module HCOPTO0014
					  and made speed enhancements by using hardware SPI 
					  interface.

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

#include "Arduino.h"
#include "HCMAX7219.h"
#include "SPI.h"

/* Constructor to initialise the GPIO and library */
HCMAX7219::HCMAX7219(byte LOAD)
{
  /* Enable the hardware SPI interface */
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  
/* Set DIO pins for interfacing to the driver as outputs */
  _LOAD = LOAD;
  pinMode(LOAD, OUTPUT); 
  digitalWrite(LOAD, HIGH);
  
  /* Initialise the driver(s) */
  Init();
  
  /* Turn text invert off */
  _InvertState = INVERTOFF;
}

#ifdef SEVENSEGMENT

/* Loads a text string into the output buffer using the seven segment 
   character set */
void HCMAX7219::print7Seg(char TextString[], unsigned int Offset)
{
  unsigned int _StringLength;
  unsigned int bufferindex;
  byte charindex;
  
  _StringLength = strlen(TextString);
  
  /* Set output buffer pointer */
  if(Offset < DISPLAYBUFFERSIZE)
  {
    bufferindex = Offset;
  }else
  {
    bufferindex = DISPLAYBUFFERSIZE;
  }
  
  /* If text runs beyond the output buffer then crop it */
  charindex = 0;
  if (Offset > DISPLAYBUFFERSIZE)
    charindex = Offset - (DISPLAYBUFFERSIZE);

  /* Copy text into output buffer */
  while(bufferindex != 0 && charindex != _StringLength)
  {
     bufferindex--;
	 DisplayBuffer[bufferindex] = SevenSegChar[TextString[charindex] - 32];
     charindex++;
  } 
}

/* Loads an integer into the output buffer using the seven segment 
   character set */
void HCMAX7219::print7Seg(long number, unsigned int Offset)
{
  char Digits[10];
  byte index = 0;
  long Temp;
  unsigned int bufferindex;

  Temp = number;
  
 /*Is the number negative ? If so then remove the sign */
  if (Temp < 0)
    Temp *= -1;

  /* Is the number zero ? */
  if (Temp == 0)
  {
    Digits[index] = '0';
    index++;
  }else
  {
    /* Convert the number to an ASCII decimal string */
    while (Temp)
    {
      Digits[index] = (Temp % 10) + 48;
      Temp /= 10;
      index++;
    } 
   
    /* If the number was negative add the sign */ 
    if (number < 0)
    {
      Digits[index] = '-';
      index++;
    }
  }
  
  /* Set output buffer pointer */
  if(Offset < DISPLAYBUFFERSIZE)
  {
    bufferindex = Offset;
  }else
  {
    bufferindex = DISPLAYBUFFERSIZE;
  } 
   
  /* If text runs beyond the output buffer then crop it */ 
  if (Offset > DISPLAYBUFFERSIZE)
    index = index - (Offset - DISPLAYBUFFERSIZE);
   
  /* Copy text into output buffer */
  while(index && bufferindex)
  {
    index--;
    bufferindex--;
    DisplayBuffer[bufferindex] = SevenSegChar[Digits[index]-32];
  }
}


/* Loads a decimal number into the output buffer using the seven segment 
   character set */
void HCMAX7219::print7Seg(long number, byte decimalPlace, unsigned int Offset)
{

  char Digits[10];
  byte index = 0;
  long Temp;
  unsigned int bufferindex;

  Temp = number;
  
 /*Is the number negative ? If so then remove the sign */
  if (Temp < 0)
    Temp *= -1;

  /* Is the number zero ? */	
  if (Temp == 0)
  {
    Digits[index] = '0';
    index++;
  }else
  {
	/* Convert the number to an ASCII decimal string */
    while (Temp)
    {
      Digits[index] = (Temp % 10) + 48;
      Temp /= 10;
      index++;
    } 
  }
  
  /* If decimal place is at the beginning of the number then pad it 
     with a zero */
  if (decimalPlace == index)
  {
    Digits[index] = '0';
    index++;
  }
   
  /* If the number was negative add the sign */ 
  if (number < 0)
  {
    Digits[index] = '-';
    index++;
  }
  
  /* Set output buffer pointer */
  if(Offset < DISPLAYBUFFERSIZE)
  {
    bufferindex = Offset;
  }else
  {
    bufferindex = DISPLAYBUFFERSIZE;
  } 
   
  /* If text runs beyond the output buffer then crop it */  
  if (Offset > DISPLAYBUFFERSIZE)
    index = index - (Offset - DISPLAYBUFFERSIZE);

  /* Copy text into output buffer */
  while(index && bufferindex)
  {
    index--;
    bufferindex--;
    if(decimalPlace !=0 && index == decimalPlace)
    {
      DisplayBuffer[bufferindex] = SevenSegChar[Digits[index]-32] |  SevenSegChar[14];
    }else
    {
      DisplayBuffer[bufferindex] = SevenSegChar[Digits[index]-32];
    } 
  }
}
#endif




/* Loads a text string into the output buffer using the seven segment 
   character set */
#ifdef DOTMATRIX
void HCMAX7219::printMatrix(char TextString[], unsigned int Offset)
{
  unsigned int bufferindex;
  unsigned int charindex;
  unsigned int _StringLength;

  _StringLength = strlen(TextString);
  
  if(Offset < DISPLAYBUFFERSIZE)
  {
    bufferindex = Offset;
  }else
  {
    bufferindex = DISPLAYBUFFERSIZE;
  }

  /*If text runs beyond the output buffer then crop it */
  charindex = 0;
  if (Offset > DISPLAYBUFFERSIZE)
    charindex = Offset - (DISPLAYBUFFERSIZE);
  
  /* Copy the text into the buffer */
  while(bufferindex > 0 && CalCharIndex(charindex) < _StringLength)
  {
    bufferindex--;
	DisplayBuffer[bufferindex] = _InvertState ^ Font8x8[TextString[CalCharIndex(charindex)] - 32][charindex%8];
	charindex++;
  }  
}



/* Loads a decimal number into the output buffer using the dot matrix segment 
   character set */
void HCMAX7219::printMatrix(long number, unsigned int Offset)
{
  char Digits[10];
  unsigned int charindex;
  byte index = 0;
  long Temp;
  unsigned int bufferindex;

  Temp = number;
  
  /*Is the number negative ? If so then remove the sign */
  if (Temp < 0)
    Temp *= -1;

  /* Is the number zero ? */	
  if (Temp == 0)
  {
    Digits[index] = '0';
    index++;
  }else
  {
	/* Convert the number to an ASCII decimal string */
    while (Temp)
    {
      Digits[index] = (Temp % 10) + 48;
      Temp /= 10;
      index++;
    } 
  }
  

  /* If the number was negative add the sign */ 
  if (number < 0)
  {
    Digits[index] = '-';
    index++;
  }
  
  /* Set output buffer pointer */
  if(Offset < DISPLAYBUFFERSIZE)
  {
    bufferindex = Offset;
  }else
  {
    bufferindex = DISPLAYBUFFERSIZE;
  } 

  charindex = index * 8;
  
  /*If text runs beyond the output buffer then crop it */
  if (Offset > DISPLAYBUFFERSIZE)
  { 
    if(Offset - DISPLAYBUFFERSIZE < charindex)
	{
		charindex = charindex - (Offset - DISPLAYBUFFERSIZE);
	}else
	{
		charindex = 0;
	}
  } 

  /* Copy the text into the buffer */
  while(bufferindex && charindex)
  {
  	charindex--;
    bufferindex--;
	if(CalCharIndex(charindex) <= index)
	  DisplayBuffer[bufferindex] = _InvertState ^ Font8x8[Digits[CalCharIndex(charindex)] - 32][7- (charindex%8)];
  } 
}


/* Loads a decimal number into the output buffer using the dot matrix segment 
   character set */
void HCMAX7219::printMatrix(long number, byte decimalPlace, unsigned int Offset)
{
  char Digits[10];
  unsigned charindex;
  byte index = 0;
  long Temp;
  unsigned int bufferindex;

  Temp = number;
  
  /*Is the number negative ? If so then remove the sign */
  if (Temp < 0)
    Temp *= -1;

  /* Is the number zero ? */	
  if (Temp == 0)
  {
    Digits[index] = '0';
    index++;
  }else
  {
	/* Convert the number to an ASCII decimal string */
    while (Temp)
    {
	  if(index == decimalPlace)
	  {
		Digits[index] = '.';
		index++;
	  }
      Digits[index] = (Temp % 10) + 48;
      Temp /= 10;
      index++;
    } 
  }
  
  /* If decimal place is at the beginning of the number then pad it 
     with a zero */
  if (decimalPlace == index)
  {
    Digits[index] = '.';
    index++;
	Digits[index] = '0';
    index++;
  }
   
  /* If the number was negative add the sign */ 
  if (number < 0)
  {
    Digits[index] = '-';
    index++;
  }
  
  /* Set output buffer pointer */
  if(Offset < DISPLAYBUFFERSIZE)
  {
    bufferindex = Offset;
  }else
  {
    bufferindex = DISPLAYBUFFERSIZE;
  } 

  charindex = index * 8;
  
  /*If text runs beyond the output buffer then crop it */
  if (Offset > DISPLAYBUFFERSIZE)
  { 
    if(Offset - DISPLAYBUFFERSIZE < charindex)
	{
		charindex = charindex - (Offset - DISPLAYBUFFERSIZE);
	}else
	{
		charindex = 0;
	}
  } 
   
  /* Copy the text into the buffer */
  while(bufferindex && charindex)
  {
  	charindex--;
    bufferindex--;
	  DisplayBuffer[bufferindex] = _InvertState ^ Font8x8[Digits[CalCharIndex(charindex)] - 32][7- (charindex%8)];
  } 
}


int HCMAX7219::CalCharIndex(int Offset)
{
  int CharIndex;
  if (Offset)
  {
	CharIndex = Offset - 1;
  }else
  {
	CharIndex = Offset;
  }
  return CharIndex / 8;
}
#endif

/* Set font invert flag */
#ifdef DOTMATRIX
void HCMAX7219::Invert(byte InvertState)
{
  _InvertState = InvertState;
}
#endif

/* Initialise the Maxim driver(s) */
void HCMAX7219::Init(void)
{
  byte DriverIndex;
  
  for (DriverIndex = 0; DriverIndex < NUMBEROFDRIVERS; DriverIndex++)
  {
    Write(MAX7219DECODE, 0,DriverIndex);
    Intensity(0x0F, DriverIndex);
    SevenSegDigits(7, DriverIndex);
    TestMode(TESTMODEOFF, DriverIndex);
    Shutdown(MAX7219ON, DriverIndex);
    Clear();
	Refresh();
  }
}

/* Clear the output buffer */
void HCMAX7219::Clear(void)
{
  unsigned int BufferIndex;
  
  for (BufferIndex = 0; BufferIndex < DISPLAYBUFFERSIZE; BufferIndex++)
  {
	DisplayBuffer[BufferIndex] = 0;
  }
}

/* Write to one of the drivers registers. No-ops are sent to all other 
   drivers in the chain.
   Driver is the driver number in the chain */
void HCMAX7219::Write(byte Address, byte Data, byte Driver)
{
  byte DriverIndex;

  digitalWrite(_LOAD, LOW);

  for (DriverIndex = 0; DriverIndex < NUMBEROFDRIVERS; DriverIndex++)
  {
    if(DriverIndex == Driver)
    {
	  SPI.transfer(Address); 
	  SPI.transfer(Data);
    }else
    {
	  SPI.transfer(MAX7219NOOP); 
	  SPI.transfer(0); 
    } 
  }
  digitalWrite(_LOAD, HIGH);
  digitalWrite(_LOAD, LOW);
}


/* Put a driver into shutdown mode. Valid values for Mode are:
   MAX7219OFF (shutdown mode)
   MAX7219ON (normal operation)
   Driver is the driver number in the chain */
void HCMAX7219::Shutdown(boolean Mode, byte Driver)
{
  Write(MAX7219SHUTDOWN, Mode, Driver); 
}

/* Put the driver into test mode. All outputs are turned on. 
   Valid values for mode are:
   TESTMODEOFF (test mode off)
   TESTMODEON (test mode on)
   Driver is the driver number in the chain */
void HCMAX7219::TestMode(boolean Mode, byte Driver)
{
  Write(MAX7219TEST, Mode, Driver);
}

/* Sets the number of digits to drive in each driver. 
   Valid values for Digits are:
   0 (Digit 0)
   1 (Digit 0 & 1)
   2 (Digit 0, 1 & 2)
   3 (Digit 0, 1, 2, & 3)
   4 (Digit 0, 1, 2, 3 & 4)
   5 (Digit 0, 1, 2, 3, 4 & 5)
   6 (Digit 0, 1, 2, 3, 4, 5, & 6)
   7 (Digit 0, 1, 2, 3, 4, 5,, 6 & 7)
   Driver is the driver number in the chain */
void HCMAX7219::SevenSegDigits(byte Digits, byte Driver)
{
  Write(MAX7219SCANLIMIT, Digits, Driver);
}

/* Set the intensity of the LED's. 
   Valid values for Level are 0 (min) to 0x0F (max)
   Driver is the driver number in the chain */
void HCMAX7219::Intensity(byte Level, byte Driver)
{
  Write(MAX7219INTESITY, Level, Driver);
}

/* Update all drivers with the contents of the output buffer */
void HCMAX7219::Refresh(void)
{
  byte DriverIndex;
  byte Digit;
  
  Digit = 0;
  DriverIndex = 0;

  /* Write to each of the 8 digit registers in each driver */ 
  while(DriverIndex < DISPLAYBUFFERSIZE)
  {
    Write(Digit + MAX7219DIGIT1, DisplayBuffer[Digit + (DriverIndex * 8)], DriverIndex);
    Digit++;
    if (Digit == 8)
    {
      Digit = 0;
      DriverIndex++;   
    }
  }
}