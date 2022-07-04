
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "lcdpcf8574/lcdpcf8574.h"
#define UART_BAUD_RATE 2400
#include "uart/uart.h"
#include "dht11/DHT.h"

#include <string.h>


int main(void)
{
	double temperature[1];
	double humidity[1];


	//init uart
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );

	sei();

	//init lcd
	lcd_init(LCD_DISP_ON_BLINK);

    //lcd go home
    lcd_home();

    uint8_t led = 0;
    lcd_led(led); //set led

	//Setup
	DHT_Setup();


    while(1) {
    	char buf[10];
        char buf2[10];
		//Read from sensor
		DHT_Read(temperature, humidity);
		int temp = temperature[0];
		int humi = humidity[0];
		//Check status
		switch (DHT_GetStatus())
		{
			case (DHT_Ok):
				//Print temperature
				//print(temperature[0]);
				lcd_gotoxy(0,0);
				lcd_puts("T:");
				itoa(temp,buf,10);
				uart_puts("T:");
				uart_puts(buf);
				lcd_gotoxy(3,0);
				lcd_puts(buf);
				lcd_putc((char)0b11011111);
				lcd_puts("C");

				//Print humidity
				//print(humidity[0]);
				lcd_gotoxy(0, 1);
				lcd_puts("H:");
				uart_puts("H:");
				itoa(humi,buf2,10);
				lcd_gotoxy(3, 1);
				uart_puts(buf2);
				uart_puts("\n");
				lcd_puts(buf2);
				lcd_putc((char)0b00100101);
				break;
			case (DHT_Error_Checksum):
				//Do something
				uart_puts("Error Checksum\n");
				break;
			case (DHT_Error_Timeout):
				//Do something else
				uart_puts("Error Timeout\n");
				break;
			default:
				uart_puts("Other Error\n");
		}

       	_delay_ms(2000);

    	}
}



