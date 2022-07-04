/*
 * File:main.c
 * Description:
 * TG11 test transmitted and receive
 * main problem is on wire...
 * author: Lichmore
 * date:2021/09/27
 */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define UART_BAUD_RATE 2400
#include "uart/uart.h"
#include <string.h>

int main(void){

	//init uart
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );

	sei();//start the interrupt

	DDRA |= 0xFF;//set port A as output;
	DDRB |= 0x01; //set PB0 as output EN
	PORTB |= 0x00;//set LOW
	DDRC = 0xFF;

	while(1){
		int i,address;
		for(address=0;address<0xFF;address++){ // different address
			PORTC = address;
			for(i = 0;i<16;i++){ // different signal
				char buf[10];
				PORTA = i;
				itoa(i,buf,10);
				uart_puts(buf);
				_delay_ms(50);

			}
		}
	}
}
