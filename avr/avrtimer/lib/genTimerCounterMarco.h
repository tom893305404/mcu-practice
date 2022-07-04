/*
 * genTimerCounter.h
 *	timer0 has two function,the behavior is select by CLK_SRC
 *  Created on: 2021年9月27日
 *      Author: Lichmore
 */
#ifndef LIB_GENTIMERCOUNTERMARCO_H_
#define LIB_GENTIMERCOUNTERMARCO_H_

/*
 * 時鐘源選擇 for timer 0/1
 * timer0 & timer1 is the same
 * use for set TCCR0 and TCCR1B less 3 bits
 * CSx2 CSx1 CSx0
 */
#define NO_CLK_SOURCE				0x00		//無時鐘源
#define CLK_SRC_AS_CPU				0x01		//F_CPU
#define CLK_SRC_8 		 			0x02		//F_CPU / 8
#define CLK_SRC_64					0x03		//F_CPU / 64
#define CLK_SRC_256					0x04		//F_CPU / 256
#define CLK_SRC_1024				0x05		//F_CPU / 1024
#define CLK_SRC_EXTERNAL_FALL		0x06		//外部時鐘源 T0/1，下降源觸發
#define CLK_SRC_EXTERNAL_RISE		0x07		//系統時鐘 T0/1 ，上升源觸發
//end of 時鐘源選擇 for timer 0/1

/*
 * 時鐘源選擇 for timer 2
 * timer0 & timer1 is the same
 * use for set TCCR0 and TCCR1B less 3 bits
 * CS22 CS21 CS20
 */
#define NO_CLK_SOURCE_2						0x00		//no clk src(stop)
#define CLK_SRC_AS_CPU_2					0x01		//F_CPU
#define CLK_SRC_8_2							0x02		//F_CPU / 8
#define CLK_SRC_32_2						0x03		//F_CPU / 32
#define CLK_SRC_64_2						0x04		//F_CPU / 64
#define CLK_SRC_128_2						0x05		//F_CPU / 128
#define CLK_SRC_256_2						0x06		//F_CPU / 256
#define CLK_SRC_1024_2						0x07		//F_CPU / 1024
//end of 時鐘源選擇 for timer 2

//---------------------------------------------------------------------//
/*	any Registers about timer counter
 * 	1.TCCR0 --> 時鐘源與功能
 * 	2.TCNTx 計數器寄存器 主要數的地方
 * 	ex. mega8 TCNT0 8 bit when 255 to 0 rise interrupt
 * 	3.TIFR 標示中斷發生 flag
 *	4.TIMSK 開關中斷
 *	5.SFIOR Reset Prescalar

 * 功能:在一般模式下方便計算波型在特定占空比下個需要多少來設定TCNT
 * freq 你期望產生的頻率
 * prescaler 你所設定的預分頻器比率
 * duty 你期望的占空比
 * dis 結果輸出[0]=high [1]=low 的時間
 * *注意:這只適用於8 bit time因為預設TCNT=256來計算*
 */
extern void duty_cycle(int freq,int prescaler,float duty,int* dis);
#endif /* LIB_GENTIMERCOUNTERMARCO_H_ */
