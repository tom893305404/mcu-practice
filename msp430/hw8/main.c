#include <msp430.h>
#include "hw.h"



int deFlag(unsigned int * flag); //防彈跳
void light_ctl_setup(void);
void hall_ctl_setup(void);
void hall_handler();
void pwm_handler(unsigned int U_H,unsigned int U_L,unsigned int V_H,unsigned int V_L,unsigned int W_H,unsigned int W_L);
void light_ctl_logic(void);
void SetVCoreUp(unsigned int level);
void CLKinit();
void PWM_cycle(volatile unsigned int *address,int Per);
void timer_setup(void);
void adc_setup(void);



/*global variable*/
unsigned int fHead, fLeft, fRight, fBrake;
unsigned int pwm_value          =       0;
unsigned int current_value      =       0;
unsigned int voltage_value      =       0;
unsigned int hall_value         =       0;
unsigned int pwm_value_z        =       0;
unsigned int current_value_z    =       0;
unsigned int voltage_value_z    =       0;

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	/*初始化設定*/
	CLKinit();
	timer_setup(); // timer0 PWM setup
	adc_setup();//A0　A1  A2
	light_ctl_setup();
	hall_ctl_setup();

//	/*for test*/
//	P3DIR |= (BIT2);
//	P3SEL &= ~(BIT2);
//	P3DIR |= (BIT1);
//    P3SEL &= ~(BIT1);


    __bis_SR_register(GIE); //開 global 中斷 Flag

    UCSCTL7 = UCSCTL7 & (~BIT3)
                & (~BIT2) & (~BIT1) & (~BIT0);
    //ADC做開始
    ADC12CTL0 = ADC12CTL0 | (BIT4);//ADC12ON
    ADC12CTL0 = ADC12CTL0 | (BIT1);//ADC12ENC

    while(1){
        //P2IN & (BIT0) Y   1
        //P2IN & (BIT6) G   32
        //P2IN & (BIT3) B   8
        hall_value = P2IN & 73; //hall判斷邏輯
        light_ctl_logic();//light logic


	 }
}

void adc_setup(void){
    REFCTL0 = REFCTL0 & (~BIT7);// set refmstr = 0
    P6SEL = P6SEL | (BIT0) | (BIT1) | (BIT2);
        // p6.0 p6.1 p6.2 as peripheral module function
    P6DIR = P6DIR & (~BIT0) & (~BIT1) & (~BIT2);
        // set P6.0 p6.1 p6.2 as input
    //ADC12CTL0
    ADC12CTL0 = ADC12CTL0 & (~BIT3);
        //ADC12OVIE=0 disable overflow interrupt
    ADC12CTL0 = ADC12CTL0 & (~BIT2);
        //ADC12TOVIE=0 disable conversion time overflow
    ADC12CTL0 = ADC12CTL0 & (~BIT1);
        //ADC12ENC=0 not enable
    ADC12CTL0 = ADC12CTL0 & (~BIT0);
        //ADC12SC=0 not start conversion
    ADC12CTL0 = ADC12CTL0 & (~0xF000);
        // ADC12SHT1x as 0000 for ADC12CLK (4 adc clk)
    ADC12CTL0 = ADC12CTL0 & (~0x0F00);
        // ADC12SHT0x as 0000 for ADC12CLK (4 adc clk)
    ADC12CTL0 = ADC12CTL0 | (BIT7);
        //ADC12MSC = 1
    ADC12CTL0 = ADC12CTL0 | (BIT6);
        //ADC12REF2_5 = 1
    ADC12CTL0 = ADC12CTL0 | (BIT5);
        //ADC12REFON = 1
    ADC12CTL0 = ADC12CTL0 & (~BIT4);
        //ADC12ON = 0 ADC OFF
    //ADC12CTL1
    ADC12CTL1 = ADC12CTL1 & (~BITF) & (~BITE) & (~BITD) & (~BITC);
        // ADC12CSTARTADDx 0000 for start convert from ADC12MEM0
    ADC12CTL1 = ADC12CTL1 & (~BITB) & (~BITA);
        //ADC12SHSx as 00 for ADC12SC bit as s/h source
    ADC12CTL1 = ADC12CTL1 | (BIT9);
        //ADC12SHP as 1 pulse mode is from sampling timer
    ADC12CTL1 = ADC12CTL1 & (~BIT8);
        //ADC12ISSH as 0 sample input is not inverted
    ADC12CTL1 = ADC12CTL1 & (~BIT7) | (BIT6) & (~BIT5);
        //ADC12DIVx as 010 adc clock is divided by 3
    ADC12CTL1 = ADC12CTL1 | (BIT4) & (~BIT3);
        //ADC12SSELx as 10 adc clock source as MCLK
    ADC12CTL1 = ADC12CTL1 & (~BIT2) | (BIT1);
        //ADCCONSEQx as 01 for sequence channel single conv
    ADC12CTL1 = ADC12CTL1 & (~BIT0);
    //ADC12CTL2
    ADC12CTL2 = ADC12CTL2 | (BIT8);
        //ADC12PDIV as 1 predivider by 4
    ADC12CTL2 = ADC12CTL2 | (BIT7);
        //ADC12TCOFF as 1 temp sensor off
    ADC12CTL2 = ADC12CTL2 | (BIT5) & (~BIT4);
        //ADC12RES as 10 resolution 12 bits
    ADC12CTL2 = ADC12CTL2 & (~BIT3);
        //ADC12DF as 0 bit unsigned binary
    ADC12CTL2 = ADC12CTL2 & (~BIT2);
        //ADC121SR as 0 buffer support up to 200kbps
    ADC12CTL2 = ADC12CTL2 | (BIT1);
        //ADC12REFOUT as 1 reference output on
    ADC12CTL2 = ADC12CTL2 & (~BIT0);
        //ADC12REFBURST as 0 reference output continuously
    //ADC12MCTL0
    ADC12MCTL0 = ADC12MCTL0 & (~BIT7);
        //ADC12EOS as 0 NOT end of sequence
    ADC12MCTL0 = ADC12MCTL0 & (~BIT6) & (~BIT5) | (BIT4);
        //ADC12SREFx as 001 reference selection as Vref+ to AVSS
    ADC12MCTL0 = ADC12MCTL0 & (~BIT3) & (~BIT2) & (~BIT1) & (~BIT0);
        //ADC12INCHx as 0000 select A0
    //ADC12MCTL1
    ADC12MCTL1 = ADC12MCTL1 & (~BIT7);
        //ADC12EOS as 0 NOT end of sequence
    ADC12MCTL1 = ADC12MCTL1 & (~BIT6) & (~BIT5) | (BIT4);
        //ADC12SREFx as 001 reference selection as Vref+ to AVSS
    ADC12MCTL1 = ADC12MCTL1 & (~BIT3) & (~BIT2) & (~BIT1) | (BIT0);
        //ADC12INCHx as 0001 select A1
    //ADC12MCTL2
    ADC12MCTL2 = ADC12MCTL2 | (BIT7);
        //ADC12EOS as 1 end of sequence
    ADC12MCTL2 = ADC12MCTL2 & (~BIT6) & (~BIT5) | (BIT4);
        //ADC12SREFx as 001 reference selection as Vref+ to AVSS
    ADC12MCTL2 = ADC12MCTL2 & (~BIT3) & (~BIT2) | (BIT1) & (~BIT0);
        //ADC12INCHx as 0010 select A2
    //ADC12IE
    ADC12IE = 0x0007;//interrupt enable A2

}
void PWM_cycle(volatile unsigned int *address,int Per){
    //TA0CCR1 = Percentage;
    *address = Per;
}

void timer_setup(void){
    //Timer A0 --------------------------------------------
     //timer A0 IO
     P1SEL = P1SEL | (BIT5) | (BIT4) | (BIT3) | (BIT2);//p1.2 p1.3 p1.4 p1.5 as Peripheral module function
     P1DIR = P1DIR | (BIT5) | (BIT4) | (BIT3) | (BIT2);//p1.2 p1.3 p1.4 p1.5 as output
     //timer A0-------------------------------------------
     TA0CTL = TA0CTL | (BIT2);//TAR as 1, clear TAR
     TA0CTL = TA0CTL | (BIT9) & (~BIT8); //TASSEL as 10, (SMCLK)(12.5Mhz)
     TA0CTL = TA0CTL & (~BIT7) & (~BIT6);//ID as 00, (input divider as /1)12.5Mhz
     TA0CTL = TA0CTL & (~BIT5) | (BIT4);//MC as 01, (mode control as up mode)上數
     TA0CTL = TA0CTL & (~BIT1); //TAIE as 0, interrupt disable
     TA0CTL = TA0CTL & (~BIT0);//TAIFG as 0, not interrupt flag is pending
     TA0R = 0x0000;
     TA0CCR0 = DUTY;//1040 (period = 1024xT_SMCLK)
     //TA0R
     TA0EX0 = TA0EX0 & (~BIT2) & (~BIT1) & (~BIT0);//TAIDEX as 000, (divided by 1)12.5MHZ

     //timer A0.0
     TA0CCTL0 = TA0CCTL0 | (BIT4);//enable TA0 CCR0 interrupt                       <<0

     //timer A0.1 p1.2
     TA0CCR1 = 0x0000; //0 initial value for high side
     TA0CCTL1 = TA0CCTL1 & (~BIT8); //CAP as 0, (compare mode PWM)
     TA0CCTL1 = TA0CCTL1 & ~(0b111<<5); TA0CCTL1 = TA0CCTL1 | (0b011<<5);//reset/set
     TA0CCTL1 = TA0CCTL1 & (~BIT4); //disable TA0 CCR1 interrupt                   <<1

     //timer A0.2 p1.3
     TA0CCR2 = 0x0000; //0 initial value for high side
     TA0CCTL2 = TA0CCTL2 & (~BIT8); //CAP as 0, (compare mode PWM)
     TA0CCTL2 = TA0CCTL2 & ~(0b111<<5); TA0CCTL2 = TA0CCTL2 | (0b011<<5);//reset/set
     TA0CCTL2 = TA0CCTL2 & (~BIT4); //disable TA0 CCR1 interrupt                    <<2

     //timer A0.3 p1.4
     TA0CCR3 = 0x0000; //0 initial value for high side
     TA0CCTL3 = TA0CCTL3 & (~BIT8); //CAP as 0, (compare mode PWM)
     TA0CCTL3 = TA0CCTL3 & ~(0b111<<5); TA0CCTL3 = TA0CCTL3 | (0b011<<5);//reset/set
     TA0CCTL3 = TA0CCTL3 & (~BIT4); //disable TA0 CCR1 interrupt                    <<3

     //timer A0.4  p1.5
     TA0CCR2 = 0x0000; //0 initial value for high side
     TA0CCTL4 = TA0CCTL4 & (~BIT8); //CAP as 0, (compare mode PWM)
     TA0CCTL4 = TA0CCTL4 & ~(0b111<<5); TA0CCTL4 = TA0CCTL4 | (0b011<<5);//reset/set
     TA0CCTL4 = TA0CCTL4 & (~BIT4); //disable TA0 CCR1 interrupt                <<4

     //Timer A2 --------------------------------
     //timer A2 IO
     P2SEL = P2SEL | (BIT5) | (BIT4);//p2.4 p2.5 as Peripheral module function
     P2DIR = P2DIR | (BIT5) | (BIT4);//p2.4 p2.5 as output

     //timer A2-------------------------------------------
     TA2CTL = TA2CTL | (BIT2);//TAR as 1, clear TAR
     TA2CTL = TA2CTL | (BIT9) & (~BIT8);//TASSEL as 10, (SMCLK)(12.5Mhz)
     TA2CTL = TA2CTL & (~BIT7) & (~BIT6);//ID as 00, (input divider as /1)12.5Mhz
     TA2CTL = TA2CTL & (~BIT5) | (BIT4); //MC as 01, (mode control as up mode)上數mode
     TA2CTL = TA2CTL & (~BIT1);//TAIE as 0, interrupt disable
     TA2CTL = TA2CTL & (~BIT0);//TAIFG as 0, not interrupt flag is pending

     TA2R = 0x0000;
     TA2CCR0 = DUTY;//

     TA2EX0 = TA2EX0 & (~BIT2) & (~BIT1) & (~BIT0);//TAIDEX as 000, (divided by 1)12.5MHZ

     //timer A2.0
     TA2CCTL0 = TA2CCTL0 & (~BIT4);//disable TA2 CCR0 interrupt             <<0_2

     //timer A2.1 p2.4
     TA2CCR1 = 0x0000; //0 initial value for high side
     TA2CCTL1 = TA2CCTL1 & (~BIT8); //CAP as 0, (compare mode PWM)
     TA2CCTL1 = TA2CCTL1 & ~(0b111<<5); TA2CCTL1 = TA2CCTL1 | (0b011<<5);//reset/set
     TA2CCTL1 = TA2CCTL1 & (~BIT4); //disable TA0 CCR1 interrupt            <<5
     //timer A2.2 p2.5
     TA2CCR2 = 0x0000; //0 initial value for high side
     TA2CCTL2 = TA2CCTL2 & (~BIT8); //CAP as 0, (compare mode PWM)
     TA2CCTL2 = TA2CCTL2 & ~(0b111<<5); TA2CCTL2 = TA2CCTL2 | (0b011<<5);//reset/set
     TA2CCTL2 = TA2CCTL2 & (~BIT4); //disable TA0 CCR1 interrupt            <<6

     //Timer A clear (timer synchronous)(initial)--------------------------------
     TA0CTL = TA0CTL | (BIT2);//clear TA0 counter
     TA2R = 0x0001;

     TA0CCR1 = 0x0000;//000
     TA0CCR2 = 0x0000;//520
     TA0CCR3 = 0x0000;//000
     TA0CCR4 = 0x0000;//520
     TA2CCR1 = 0x0000;//000
     TA2CCR2 = 0x0000;//520

     TA0CTL = TA0CTL & (~BIT0);//clear TA0 interrupt flag
     TA0CCTL0 = TA0CCTL0 & (~BIT0);//clear CCR0 CCIFG flag
     TA0CCTL1 = TA0CCTL1 & (~BIT0);//clear CCR1 CCIFG flag
     TA0CCTL2 = TA0CCTL2 & (~BIT0);//clear CCR2 CCIFG flag
     TA0CCTL3 = TA0CCTL3 & (~BIT0);//clear CCR3 CCIFG flag
     TA0CCTL4 = TA0CCTL4 & (~BIT0);//clear CCR4 CCIFG flag
     TA2CCTL0 = TA2CCTL0 & (~BIT0);//clear CCR0 CCIFG flag
     TA2CCTL1 = TA2CCTL1 & (~BIT0);//clear CCR1 CCIFG flag
     TA2CCTL2 = TA2CCTL2 & (~BIT0);//clear CCR2 CCIFG flag

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
void CLKinit()
{
// setup core voltage up------------------------------
    SetVCoreUp(0);
    SetVCoreUp(1);
    SetVCoreUp(2);
    SetVCoreUp(3);

//Unified Clock System --------------------------------
    //MCLK       SMCLK          ACLK
    //MCLK 25M         12.5M      10k
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
void hall_ctl_setup(void){
    // hall p2.3 input B
        P2DIR &= ~(BIT3);       //as input
        P2REN &= ~(BIT3);        //pullup enable;
        P2OUT &= ~(BIT3);        //pullup
     //end of p2.3

    // hall p2.6 input G
        P2DIR &= ~(BIT6);
        P2REN &= ~(BIT6);        //pullup enable;
        P2OUT &= ~(BIT6);        //pullup
     //end of p2.6

    // hall p2.0 input Y
        P2DIR &= ~(BIT0);
        P2REN &= ~(BIT0);        //pullup enable;
        P2OUT &= ~(BIT0);        //pullup
     //end of p2.0
}

void hall_handler(){
    //P2IN & (BIT0) Y   1   U
    //P2IN & (BIT6) G   64  V
    //P2IN & (BIT3) B   8   W
   // int encode = (hall_value & BIT0) + ((hall_value & BIT6) >> 5) +  ((hall_value & BIT3)>>)
    switch(hall_value){
    case 64:
        pwm_handler(SECTION1);
            break;
    case 8:
        pwm_handler(SECTION2);
            break;
    case 72:
        pwm_handler(SECTION3);
            break;
    case 1:
        pwm_handler(SECTION4);
            break;
    case 65:
        pwm_handler(SECTION5);
            break;
    case 9:
        pwm_handler(SECTION6);
            break;
    default:
        break;
    }
}
void pwm_handler(unsigned int U_H,unsigned int U_L,unsigned int V_H,unsigned int V_L,unsigned int W_H,unsigned int W_L){
    unsigned int pwm = pwm_value;

    TA0CCR1 = POS_LOW;
    TA0CCR3 = POS_LOW;
    TA2CCR1 = POS_LOW;
    TA0CCR2 = POS_LOW;
    TA0CCR4 = POS_LOW;
    TA2CCR2 = POS_LOW;

    if(U_H)
    {
        TA0CCR1 = pwm;
       //PWM_cycle(&TA0CCR1, pwm);
    }
    else if(V_H)
    {
       TA0CCR3 = pwm;
       //PWM_cycle(&TA0CCR3, pwm);
    }
    else if (W_H)
    {
       TA2CCR1 = pwm;
       //PWM_cycle(&TA2CCR1, pwm);
    }

    if(U_L)
    {
        TA0CCR2 = pwm;
        //PWM_cycle(&TA0CCR2, POS_HIGH);
    }
    else if(V_L)
    {
        TA0CCR4 = pwm;
        //PWM_cycle(&TA0CCR4, POS_HIGH);
    }
    else if (W_L)
    {
        TA2CCR2 = pwm;
        //PWM_cycle(&TA2CCR2, POS_HIGH);
    }



//    //p1.2
//    tmp = (U_H)? pwm:POS_LOW;
//    PWM_cycle(&TA0CCR1, tmp);
//    //p1.4
//    tmp = (V_H)? pwm:POS_LOW;
//    PWM_cycle(&TA0CCR3, tmp);
//    //p2.4
//    tmp = (W_H)? pwm:POS_LOW;
//    PWM_cycle(&TA2CCR1, tmp);
//    /****************************/
//    //p1.3
//    tmp = (U_L)? POS_HIGH:POS_LOW;
//    PWM_cycle(&TA0CCR2, tmp);
//    //p1.5
//    tmp = (V_L)? POS_HIGH:POS_LOW;
//    PWM_cycle(&TA0CCR4, tmp);
//    //p2.5
//    tmp = (W_L)? POS_HIGH:POS_LOW;
//    PWM_cycle(&TA2CCR2, tmp);
}
void light_ctl_setup(void){
    fHead = fLeft = fRight = fBrake = 1; //debounce flag init

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

}
void light_ctl_logic(void){
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



