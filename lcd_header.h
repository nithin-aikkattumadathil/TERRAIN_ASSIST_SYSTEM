
/*
 * lcd_header.h
 *
 * Created: 12/15/2022 3:09:50 PM
 *  Author: Student
 */ 
#include <util/delay.h>
#define F_CPU 16000000UL
#define lcd_port PORTD          // we have connected the lcd on port D
#define lcd_data_dir  DDRD      // we're defining the direction of the pins, weather it is input or output
#define rs PD0                  // we need total six pin to show output on the LCD. Datapin 4, 5, 6, and 7 for sending the data to the lcd and Enable and RS pin for controlling
// the behavior of the LCD we have connected RS pin of LCD to port D pin 0
#define en PD1                  // we have connected Enable pin of LCD to port D pin 1


void lcd_command( unsigned char cmnd )
{

	lcd_port = (lcd_port & 0x0F) | (cmnd & 0xF0);
	lcd_port &= ~ (1<<rs);
	lcd_port |= (1<<en);
	_delay_us(1);
	lcd_port &= ~ (1<<en);
	
	// wait 200 microseconds
	_delay_us(200);
	
	// send the lower 4 bit of the data
	lcd_port = (lcd_port & 0x0F) | (cmnd << 4);
	lcd_port |= (1<<en);
	_delay_us(1);
	lcd_port &= ~ (1<<en);
	_delay_ms(2);
}


void initialize (void)
{
	lcd_data_dir = 0xFF;     // this will set the LCD pins connected on the microcontroller as output
	_delay_ms(15);           // to show data on the LCD we need to send commands first then the data
	lcd_command(0x02);       // this command returns the cursor to the first row and first column position
	lcd_command(0x28);
	lcd_command(0x0c);
	lcd_command(0x06);
	lcd_command(0x01);
	_delay_ms(2);
}


void lcd_clear()
{
	lcd_command (0x01);   // this line clears the LCD screen
	_delay_ms(2);         // waits for two milliseconds
	lcd_command (0x80);   // this line sets the cursor to the row 1 column 1
}


void lcd_print (char *str)
{
	// this function will be used to display the string on the LCD screen
	int i;
	for(i=0; str[i]!=0; i++)
	{
		// we can not send the whole string to the LCD we need to send character by character
		// data sending is same as sending a command. there is one difference, in this case the RS pin will be set to HIGH while the RS pin was set to zero in case of the command sending
		lcd_port = (lcd_port & 0x0F) | (str[i] & 0xF0);
		lcd_port |= (1<<rs);
		lcd_port|= (1<<en);
		_delay_us(1);
		lcd_port &= ~ (1<<en);
		_delay_us(200);
		lcd_port = (lcd_port & 0x0F) | (str[i] << 4);
		lcd_port |= (1<<en);
		_delay_us(1);
		lcd_port &= ~ (1<<en);
		_delay_ms(2);
	}
}



void lcd_setCursor(unsigned char x, unsigned char y){    // this function will be used to set cursor. the place where we want to display the data
	unsigned char adr[] = {0x80, 0xC0};    // the 16x2 LCD has two rows first row has a value of 0x80. So let's say we want to go to the seconds column of first row
	// we just need to send the command with adding 2 with the initial value. So, it will be (0x80 + 2) this is how the code works
	lcd_command(adr[y-1] + x-1);
	_delay_us(100);
}
