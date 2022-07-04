/*
 * file:main.c
 * Description: EEPROM test
 * Author:lichmore
 * Date:2021/10/06
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#define UART_BAUD_RATE 2400
#include "uart/uart.h"
#include "eeprom/eeprom.h"
int main(){
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );
	sei();
	uart_puts("start...\n");
	volatile int i;
	for(i=0;i<100;i++){
		eeprom_write(i, i);
	}
	uart_puts("write finish\n");
	volatile int data[100];
	for(i=0;i<100;i++){
		data[i] = eeprom_read(i);
	}
	uart_puts("read finish\n");
	char buf[10];
	for(i=0;i<100;i++){
			data[i] = eeprom_read(i);
			itoa(data[i], buf, 10);
			uart_puts(buf);
	}

	while(1){//loop

	}
}
