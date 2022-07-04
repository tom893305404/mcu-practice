#include <msp430.h> 


/**
 * main.c
 * author:lichmore
 * homework lab5 pwm跟中斷實驗
 * io p2.2 as input p1.2(TA0.1) as timer output
 *  12kHz 25% 及 75% PWM
 *  //MCLK 25M        SMCLK  12.5M    ACLK  10k
 */
void SetVCoreUp(unsigned int level);
void CLKinit();
void PWM_cycle(float Per);
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	CLKinit();
	//setting p2.2 as INT2.2 input


	P2SEL = P2SEL & (~BIT1); //port2 bit1 as 0 digital io
	P2DIR = P2DIR & (~BIT1); //port2 bit1 as 0 input
	P2REN = P2REN | (BIT1); //port2 bit1 as 1 pull up/down input
	P2OUT = P2OUT | (BIT1); //port2 bit1 as 1 pull up input
	P2IE = P2IE | (BIT1); //port2 bit1 as 1 interrupt enable
	P2IES = P2IES | (BIT1); //port2 bit1 as 1 flag set high to low transition (高轉低觸發
	P2IFG = 0x00; //clear interrupt flag.

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
	TA0CCR0 = 1040; //1040 (period = 1024xT_SMCLK)//半個週期
	//TA0R
	TA0EX0 = TA0EX0 & (~BIT2) & (~BIT1) & (~BIT0); //TAIDEX as 000, (divided by 1)12.5MHZ

	//timer A0.1
	TA0CCR1 = 0x0000; //0 initial value for high side
	TA0CCTL1 = TA0CCTL1 & (~BIT8); //CAP as 0, (compare mode PWM)
	TA0CCTL1 = TA0CCTL1 | (BIT7) | (BIT6) | (BIT5); //OUTMOD as 111, (reset/set)
	TA0CCTL1 = TA0CCTL1 & (~BIT4); //disable TA0 CCR1 interrupt



	__bis_SR_register(GIE); // I interrupt enable
	PWM_cycle(0.1);
	while(1){}
}

void PWM_cycle(float Per){
    int Percentage = (Per * 1040);
    TA0CCR1 = Percentage;
}

int tmpp=1;
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void){//負責切換輸出為25% or 75%
    //

    if(tmpp= !tmpp){
        PWM_cycle(0.75);
    }else{
        PWM_cycle(0.25);
    }

    P2IFG = 0x00; //clear interrupt flag

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
