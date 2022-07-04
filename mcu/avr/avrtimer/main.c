/*
 *	File: main.c
 *
 *	Description:general Timer Counter library
 *
 *	author:lichmore
 *
 *	Date:2021/09/27
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "lib/gentimerCounterMarco.h"
#define UART_BAUD_RATE 115200
#include "uart/uart.h"
#include <string.h>
volatile int temp = 1;
int dis[2];

int main(){
	DDRB = 0xFF;
	PORTB = 0xFF;
	//duty_cycle(6000,8,0.9,dis);
	TCCR0 = 0x00;//stop timer
	SFIOR |= 0x01; //reset prescalar
	TCNT0 = 0;//counter 初始值
	OCR0 = 120;//預計都在ovf前觸發
	TIMSK |= 0x03;//開OC跟ovf
	sei();//I flag
	TCCR0 |= (1<<WGM00); //set WGM = 1 0 == 快速PWM
	TCCR0 |= (1<<COM01);//set OC output mode
	TCCR0 |= (1<<CS01) | (1<<CS00);//64


	while(1){

	}
}
ISR(TIMER0_COMP_vect){

}

ISR(TIMER0_OVF_vect){

}


