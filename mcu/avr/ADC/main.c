#include <avr/io.h>
#include <avr/interrupt.h>
#define UART_BAUD_RATE 2400
#include "uart/uart.h"
#include <string.h>
#include <util/delay.h>
volatile int tmp;
char buf[10];

uint16_t adc_read();

int main(void){

	//init uart
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );
	sei();//開中斷


	uart_puts("start...\n");

	ADMUX |= (1<<REFS0);//AVCC as ref refpin add capacitor
	ADCSRA |= (1<<ADEN) | (1<<ADIE) | (0x03);//enable interrupt open
	ADCSRA |= (1<<ADSC);//start
	while(1){
		_delay_ms(40);
		ADCSRA |= (1<<ADSC);
	}
}
ISR(ADC_vect){
	tmp = adc_read();
	itoa(tmp,buf,10);
	uart_puts(buf);
}

uint16_t adc_read(){
	uint8_t low,high;
	uint16_t ans;
	low = ADCL; //read low
	high = ADCH; //read high
	ans = (ADCH<<8) + low;
	return ans;
}
