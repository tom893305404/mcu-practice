/*
 * interrupt.c
 *
 *  Created on: 2021年12月23日
 *      Author: Lichmore
 */
#include <msp430.h>
#include "hw.h"

//  1/11 第一組
/*
 * 3A   736 678 635 735 604
 * 2.5A 549 562 559 484 477
 * 2A   440 403 433 294 371
 * 1.5A 313 320 273 297 346
 * 1A   290 222 275 168 171
 * 0.5A 16   80  31  25  54
 * */
//  1/11 第二組
/*
 * 3A
 * 2.5A 562
 * 2A  403
 * 1.5A 320
 * 1A 222
 * 0.5A 80
 * */

/*
 * voltage 1873
 * 12V 1785 1788 1735 1787
 *11V 1642 1640 1642
 *10V 1484 1494 1494 1493
 *9V  1329 1327 1306
 *
 * */

extern unsigned int pwm_value_z;
extern unsigned int current_value_z;
extern unsigned int voltage_value_z;
extern unsigned int pwm_value;
extern unsigned int current_value;
extern unsigned int voltage_value;
extern unsigned int hall_value;
unsigned int cnt=0, current_avg=0;
//volatile unsigned int test=0;
extern void pwm_handler(unsigned int U_H,unsigned int U_L,unsigned int V_H,unsigned int V_L,unsigned int W_H,unsigned int W_L);
#pragma vector=ADC12_VECTOR
__interrupt void ADC_0(void)
{
    // AD s/h
    if(ADC12IFG == 0x0004){
        voltage_value_z = ADC12MEM2; // vdd //讀值
        if(voltage_value_z > 1640){ //voltage protect
            current_value_z = ADC12MEM0; //current
            pwm_value_z = ADC12MEM1;     //860 748 841

            if(cnt++<8){
                current_avg += (current_value_z>>3);
            }else{
                current_value =current_avg;
                current_avg = 0;
                cnt = 0;
            }

            pwm_value = pwm_MAX - pwm_value_z;
            if(pwm_value < 170){ //大於80%保護
                pwm_value = 170; //1040 * (1-0.8)==
            }else{
                pwm_value = (pwm_value>2880)? 1040:(unsigned int)(pwm_value*0.3);
                //pwm_value = (pwm_value>2910)? 1040:(pwm_value/pwm_MAX * 1040);//小於670&&一般輸入 3580-670=2910
            }

            if(current_value_z>680){pwm_value = 430;}//電流限制3A

        }else{//voltage protect
            TA0CCR1 = POS_LOW;
            TA0CCR3 = POS_LOW;
            TA2CCR1 = POS_LOW;
            TA0CCR2 = POS_LOW;
            TA0CCR4 = POS_LOW;
            TA2CCR2 = POS_LOW;
        }
    }else{
        ADC12CTL0 = ADC12CTL0 | (BIT0);//ADC12SC = 1
    }
    ADC12IFG = 0x0000;//clear interrupt flag
}
//===========================================================
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0_CCR0(void)
{
    P3OUT |= (BIT1);
    //Duty 送 CCR
    switch(hall_value){
    case 64:
        pwm_handler(SECTION1);
            break;
    case 8:
        pwm_handler(SECTION3);
            break;
    case 72:
        pwm_handler(SECTION2);
            break;
    case 1:
        pwm_handler(SECTION5);
            break;
    case 65:
        pwm_handler(SECTION6);
            break;
    case 9:
        pwm_handler(SECTION4);
            break;
    default:
        break;
    }
    P3OUT &= ~(BIT1);
    // AD s/h
    //ADC12CTL0 = ADC12CTL0 & (~BIT0);//ADC12SC = 0
    ADC12CTL0 = ADC12CTL0 | (BIT0);//ADC12SC = 1
    //ADC12CTL0 = ADC12CTL0 & (~BIT0);//ADC12SC = 0


    TA0CCTL0 = TA0CCTL0 & (~BIT0);//clear CCR0 CCIFG flag
}

