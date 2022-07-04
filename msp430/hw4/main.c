#include <msp430.h> 

#define Debounce 36500
/**lab4 lab 2 lab3 合併
 * main.c
 * Description:homework2 for ctl the left right signed
 * author:  lichmore
 * date:2021/10/07
 */

volatile unsigned int fHead, fLeft, fRight, fBrake;


int deFlag(unsigned int * flag){
    if(*(flag) == 1){
        *(flag) = Debounce;
        return 1;
    }
    else {
        *(flag) = *flag - 1;
        return 0;
    }
}
void SetVCoreUp(unsigned int level);
void CLKinit();

int main(void)
{
    fHead = fLeft = fRight = fBrake = 1; //debounce flag init
    /*
     * TO-DO
     * ctl MCLK remember
     * MCUSR remember
     * SEL
     */
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    //voltage up for higher freq of core
    //setting freqence you want..(MHZ)
    CLKinit();
    //head light p4.3 頭燈
    P4DIR |= (BIT3); //head light pin as output
    //end of head light

    //head light ctl p4.0 頭燈開關
    P4DIR &= ~(BIT0);
    P4REN |= (BIT0);        //pullup enable;
    P4OUT |= (BIT0);        //pullup
    //end of light ctl

    //turn left light 左轉燈p8.2
    P8DIR |= (BIT2);
    P8OUT &= ~(BIT2);
    //end of left light

    //turn left light ctl 左轉燈控制p4.2
    P4DIR &= ~(BIT2);
    P4REN |= (BIT2);        //pullup enable;
    P4OUT |= (BIT2);        //pullup
    //end of turn left ctl

    //turn right light 右轉燈p3.7
    P3DIR |= (BIT7);
    P3OUT &= ~(BIT7);
    //end of turn right

    //turn right light ctl右轉燈控制p4.1
    P4DIR &= ~(BIT1);
    P4REN |= (BIT1);        //pullup enable;
    P4OUT |= (BIT1);        //pullup
    //end of turn right ctl

    //brake light 煞車燈p8.1
    P8DIR |= (BIT1);
    P8OUT &= ~(BIT1);
    //end of brale light

    //brake signal 煞車訊號p2.2
    P2DIR &= ~(BIT2);
    P2REN |= (BIT2);        //pullup enable;
    P2OUT |= (BIT2);        //pullup
    //end of brake signal

    /*
     * 頭燈p4.0、 左方向燈p4.2、 右方向燈p4.1 、煞車p2.2
     * light pin:
     *    p4.3  p8.2    p3.7    p8.1
     *
     */

    while (1){  //loop
        if(deFlag(&fHead)){//debounce
            if (P4IN & (BIT0)){//head light
            P4OUT &= ~(BIT3);     //turn off
            }else{
                P4OUT |= (BIT3);        //turn on
            }
        }
/////--------------------------------------------
        if(deFlag(&fLeft)){//debounce
            if (P4IN & (BIT2)){//left light
                P8OUT |= (BIT2);     //turn on
            }else{
                P8OUT &= ~(BIT2);     //turn off
            }
        }
/////--------------------------------------------
        if(deFlag(&fRight)){
            if (P4IN & (BIT1))//right light
            {
                P3OUT |= (BIT7);     //turn on
            }
            else
            {
                P3OUT &= ~(BIT7);     //turn off
            }
        }
/////--------------------------------------------
        if(deFlag(&fBrake)){
            if (P2IN & (BIT2))//brake light
            {
                P8OUT |= (BIT1);     //turn on
            }
            else
            {
                P8OUT &= ~(BIT1);     //turn off
            }
        }
/////--------------------------------------------
    }

    return 0;
}

void CLKinit()
{
// setup core voltage up------------------------------
    SetVCoreUp(0);
    SetVCoreUp(1);
    SetVCoreUp(2);
    SetVCoreUp(3);


    __bis_SR_register(SCG0);

    UCSCTL0 |= 0x1080; UCSCTL0 &= 0xF087;
    UCSCTL1 |= 0x0060; UCSCTL1 &= 0xFFEE;
    UCSCTL2 |= 0x02FA; UCSCTL2 &= 0x8EFA;
    UCSCTL3 |= 0x0020; UCSCTL3 &= 0xFFA8;
    UCSCTL4 |= 0x0133; UCSCTL4 &= 0xF9BB;
    UCSCTL5 |= 0x0010; UCSCTL5 &= 0xF898;
    UCSCTL6 |= 0x01C0; UCSCTL6 &= 0x2FCC;
    UCSCTL7 |= 0x0000; UCSCTL7 &= 0xFFF0;

    __bic_SR_register(SCG0);// Enable the FLL control loop //啟用FLL

}
void SetVCoreUp(unsigned int level)
{
    // Open PMM registers for write access
    PMMCTL0_H = 0xA5;
    // Make sure no flags are set for iterative sequences
//    while ((PMMIFG & SVSMHDLYIFG) == 0);
//    while ((PMMIFG & SVSMLDLYIFG) == 0);
    // Set SVS/SVM high side new level
    SVSMHCTL = SVSHE + SVSHRVL0 * level + SVMHE + SVSMHRRL0 * level;
    // Set SVM low side to new level
    SVSMLCTL = SVSLE + SVMLE + SVSMLRRL0 * level;
    // Wait till SVM is settled
    while ((PMMIFG & SVSMLDLYIFG) == 0);
    // Clear already set flags
    PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);
    // Set VCore to new level
    PMMCTL0_L = PMMCOREV0 * level;
    // Wait till new level reached
    if ((PMMIFG & SVMLIFG))
        while ((PMMIFG & SVMLVLRIFG) == 0)
            ;
    // Set SVS/SVM low side to new level
    SVSMLCTL = SVSLE + SVSLRVL0 * level + SVMLE + SVSMLRRL0 * level;
    // Lock PMM registers for write access
    PMMCTL0_H = 0x00;
}
