#include <avr/io.h>
#include <avr/interrupt.h>
#define UART_BAUD_RATE 2400
#include "uart/uart.h"
int main(){
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );
	sei();

	DDRB |= (1<<PB0); //set pb 0  1 as output
	SFIOR &= (1<<ACME);
	//ACSR |= (1<<ACIE);//enable AC and enalbe interrupt(中斷怪怪的)

	while(1){//這裡只有處理燈開關
		if(ACSR & (1<<ACO)){
			//uart_puts("PB0 ON\n");
			PORTB |= 0x01; // PB0 turn on
		}else{
			//uart_puts("PB0 OFF\n");
			PORTB &= (~0x01);//PB0 turn off
		}
	}

}
volatile int tmp=0;
ISR(ANA_COMP_vect){//ACIS1:0 == 00 上下沿都觸發
	uart_puts("here");
}
