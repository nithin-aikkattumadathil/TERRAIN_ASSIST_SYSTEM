#define F_CPU 16000000UL
#include <avr/io.h>             
#include <util/delay.h>        
#include <stdlib.h>   
#include "lcd_header.h"  
#include "ultra.h"       

int distance, previous_distance;																	
int main()
{
	initialize();           // we're initializing the LCD

	char numberString[4];   // we re defining an array of character. It will be utilized later to store integer to be displayed on the LCD screen

	while(1) {
		
		uint16_t r;
		//Set io port direction of sensor
		HCSR04Init();
		DDRC|=(1<<2)|(1<<3)|(1<<4)|(1<<5);    // we're setting all the leds as output
		DDRD|=(1<<2);                                                 // we need to set the fifth led as output separately because it is on another port
		PORTC^=255;
		_delay_ms(100);
		PORTD^=(1<<2);
		while(1)
		{
			//Send a trigger pulse
			HCSR04Trigger();               // calling the ultrasonic sound wave generator function
			//Measure the width of pulse
			r=GetPulseWidth();             // getting the duration of the ultrasound took to echo back after hitting the object
			//Handle Errors
			if(r==US_ERROR)                // if microcontroller doesn't get any pulse then it will set the US_ERROR variable to -1
			// the following code will check if there is error then it will be displayed on the LCD screen
			{
				lcd_setCursor(1, 1);      //lcd_setCursor(column, row)
				lcd_print("Error!");
			}
			else
			{
				distance=(r*0.034/2.0);	// This will give the distance in centimeters
				
				if (distance != previous_distance)    // the LCD screen only need to be cleared if the distance is changed otherwise it is not required
				{
					lcd_clear();
				}
				
				lcd_setCursor(1, 1);      // set the row and column to display the data
				lcd_print("Distance = ");
				lcd_setCursor(12, 1);      //lcd_setCursor(column, row)
				itoa(distance, numberString, 10);    // distance is an integer number, we can not display integer directly on the LCD. this line converts integer into array of character
				lcd_print(numberString);
				lcd_setCursor(14, 1);      //set the row to 1 and and column to 14 to display the data
				lcd_print(" cm");
				
				previous_distance = distance;
				_delay_ms(30);
			}
		}
		
	}
}