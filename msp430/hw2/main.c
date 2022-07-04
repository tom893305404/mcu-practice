/**
 * 控燈實驗
 * author:lichmore
 * date:2021/10/21
 * main.c
 *
 * r b w
 */
#include <msp430.h>
#define Debounce 36500
unsigned int fHead, fLeft, fRight, fBrake;
int deFlag(unsigned int * flag); //防彈跳
void SetVCoreUp(unsigned int level);//核心電壓
void CLKinit(void);//CLK初始化
void timer_setup(void);
void PWM_cycle(float Per);
int main(void)
{
    fHead = fLeft = fRight = fBrake = 1; //debounce flag init
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
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
        //end of brake light

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

    }
    void timer_setup(void){
        //Timer A0 --------------------------------------------
            //timer A0 IO
            P1SEL = P1SEL | (BIT5) | (BIT4) | (BIT3) | (BIT2);//p1.2 p1.3 p1.4 p1.5 as Peripheral module function
            P1DIR = P1DIR | (BIT5) | (BIT4) | (BIT3) | (BIT2);//p1.2 p1.3 p1.4 p1.5 as output
            //timer A0-------------------------------------------
            TA0CTL = TA0CTL | (BIT2); //TAR as 1, clear TAR
            TA0CTL = TA0CTL | (BIT9) & (~BIT8); //TASSEL as 10, (SMCLK)(12.5Mhz)
            TA0CTL = TA0CTL & (~BIT7) & (~BIT6); //ID as 00, (input divider as /1)12.5Mhz
            TA0CTL = TA0CTL & (~BIT5) | (BIT4); //MC as 01, (mode control as up mode)上數mode
            TA0CTL = TA0CTL & (~BIT1); //TAIE as 0, interrupt disable
            TA0CTL = TA0CTL & (~BIT0); //TAIFG as 0, not interrupt flag is pending
            TA0R = 0x0000;
            /*
             * SMCLK=12.5Mhz t = 0.08uS數一格
             * 12khz t = 0.000083
             *0.000083 / 2  / 0.08uS = 520
             *520
             */
            TA0CCR0 = 1040; //260 (period = 1024xT_SMCLK)//半個週期
            //TA0R
            TA0EX0 = TA0EX0 & (~BIT2) & (~BIT1) & (~BIT0); //TAIDEX as 000, (divided by 1)12.5MHZ

            //timer A0.1
            TA0CCR1 = 0x0000; //0 initial value for high side
            TA0CCTL1 = TA0CCTL1 & (~BIT8); //CAP as 0, (compare mode PWM)
            TA0CCTL1 = TA0CCTL1 | (BIT7) | (BIT6) | (BIT5); //OUTMOD as 111, (reset/set)
            TA0CCTL1 = TA0CCTL1 & (~BIT4); //disable TA0 CCR1 interrupt

    }
    void PWM_cycle(float Per){
        int Percentage = (Per * 1040);
        TA0CCR1 = Percentage;
    }

    void CLKinit()
    {
    // setup core voltage up------------------------------
        SetVCoreUp(0);
        SetVCoreUp(1);
        SetVCoreUp(2);
        SetVCoreUp(3);

    //Unified Clock System --------------------------------
        /*
         * Digital Control Oscillator
         * need change to set clk
         * FLLREFCLK -clock souce --SELREF
         * FLLREFDIV -預分頻器
         * FLLD 倍頻除頻器D
         * FLLN 倍頻除頻器N
         * fDCOCLK = D × (N + 1) × (fFLLREFCLK ÷ n)
         * fDCOCLKDIV = (N + 1) × (fFLLREFCLK ÷ n)
         * (D=2,n=1,N=380) --> FDCOCLK=25M(逼近),DIV=12.5M
         *
         */

        //MCLK 25M          12.5M      10k
        //MCLK=24MHz SMCLK=24MHz ACLK=32KHz
        __bis_SR_register(SCG0);// Disable the FLL control loop先關閉積分器
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
