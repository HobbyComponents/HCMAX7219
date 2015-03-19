/* FILE:    HCMODU0082_Serial_7_Segment_Module_Example2
   DATE:    19/03/15
   VERSION: 0.2
   
REVISIONS:

12/03/15 Created version 0.1
19/03/15 Updated to work with V0.2 of the HCMAX7219 library

This is an example of how to use the Hobby Components serial 8 digit seven 7 
segment display module (HCMODU0082). To use this example sketch you will 
need to download and install the HCMAX7921 library available from the software
section of our support forum (forum.hobbycomponents.com) or on github:
(https://github.com/HobbyComponents)

The library assumes you are using one module. If you have more than one module
connected together then you will need to change the following line in the 
libraries HCMAX7219.h header file to the number of drivers you have connected:

#define NUMBEROFDRIVERS 1 <- Change this number


PINOUT:

MODULE.....UNO/NANO.....MEGA
VCC........+5V..........+5V
GND........GND..........GND
DIN........11...........51
CS (LOAD)..10...........10
CLK........13...........52

You may copy, alter and reuse this code in any way you like, but please leave
reference to HobbyComponents.com in your comments if you redistribute this code.
This software may not be used directly for the purpose of promoting products that
directly compete with Hobby Components Ltd's own range of products.

THIS SOFTWARE IS PROVIDED "AS IS". HOBBY COMPONENTS MAKES NO WARRANTIES, 
WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, ACCURACY OR
LACK OF NEGLIGENCE. HOBBY COMPONENTS SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE
FOR ANY DAMAGES INCLUDING, BUT NOT LIMITED TO, SPECIAL, INCIDENTAL OR 
CONSEQUENTIAL DAMAGES FOR ANY REASON WHATSOEVER. */

/* Include the HCMAX7219 and SPI library */
#include <HCMAX7219.h>
#include "SPI.h"

/* Set the LOAD (CS) digital pin number*/
#define LOAD 10
 
/* Create an instance of the library */
HCMAX7219 HCMAX7219(LOAD);


void setup() 
{        
}

/* Main program */
void loop() 
{
  byte Loopcounter;
  byte Position;
  
  /* SCROLL SOME TEXT 5 TIMES BEFORE MOVING ON */
  for (Loopcounter = 0; Loopcounter <= 5; Loopcounter++)
  {
    /* We are scrolling 30 characters of text across the entire display */
    for (Position = 0; Position <= DISPLAYBUFFERSIZE + 30; Position++)
    { 
      /* Write the test to the output buffer at the position we require */
      HCMAX7219.print7Seg("HCMAX7219 SCROLLING TEXT DEMO ",Position);
      /* Send the output buffer to the display */
      HCMAX7219.Refresh();  
      delay(200);
    }
  }
  
  
  /* WE CAN ALSO DISPLAY INTEGER NUMBERS */
  
  /* Clear the output buffer */
  HCMAX7219.Clear();
  /* Write some text and output it*/
  HCMAX7219.print7Seg("INT NUM.",8);
  HCMAX7219.Refresh();
  delay(2000);
  
  /* display an example of a negative integer number */
  HCMAX7219.Clear();
  HCMAX7219.print7Seg(-1234567,8);
  HCMAX7219.Refresh();
  delay(2000);
  
  /* Clear the output buffer */ 
  HCMAX7219.Clear();
  /* Write some text and output it*/
  HCMAX7219.print7Seg("WITH DP.",8);
  HCMAX7219.Refresh();
  delay(2000);
  
  /* Show the DP in different places. Notice when the DP is at the 
    beginning the number is padded with a zero */
  HCMAX7219.Clear();
  for (Position = 1; Position <= 7; Position++)
  { 
    HCMAX7219.print7Seg(-1234567,Position,8);
    HCMAX7219.Refresh();
    delay(1000);
  }
}
