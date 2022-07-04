#include <msp430.h>

/**
 * file:main.c
 * description:normal gpio test(with debounce(delay))
 * author:  lichmore
 * date: 2021/10/05
 */

void de_delay(){
    int i;
    for(i=0;i<999999;i++);
}

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;       // stop watchdog timer

    P1DIR |= 0x01;                  // configure P1.0 as output p1.1 as input(close)
    P2DIR &= ~(BIT1);                   //p2.1 as input(open)
    //SEL no set as GPIO
    P1REN |= BIT1;  //as input pullup enable
    P2REN |= BIT1;  //as input pullup enable

    P1OUT |= BIT1; //pullup
    P2OUT |= BIT1; //pullup


    while(1)
    {
        if(P2IN & BIT1){    //open the led
            //沒按按鈕會一直跑到這裡
        }else{
            P1OUT |= 0x01;//turn on
            de_delay();     //delay avoid bounce
        }
        if(P1IN & BIT1){    //close the led
            //沒按按鈕會一直跑到這裡
        }else{
            P1OUT &= ~(0x01);  //turn off
                de_delay();
        }
    }
}
