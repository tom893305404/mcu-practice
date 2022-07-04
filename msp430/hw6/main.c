#include <msp430.h> 
//==========================================================
// EV Bicycle Lab2 test01
// This is simple ADC program for MSP430F5529
// ADC value control flash led rate
// (single channel single conversion)
//
// Created by Fuzen at 20190705
// Updated by Fuzen at 20191218 with comment
//===========================================================
#define ADC_MAX 3567
#define ADC_MIN 675
void SetVCoreUp(unsigned int level);
void CLKinit();
void PWM_cycle(float Per);
void timer_setup(void);


unsigned int adcvalue=10000;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   //stop watchdog timer

    CLKinit();
    timer_setup(); // timer0 PWN setup

    //ADC ref setup ----------------------------------------
    REFCTL0 = REFCTL0 & (~BIT7);// set refmstr = 0
    P6SEL |= (BIT1);// p6.0 as peripheral module function(p6.0外設啟用(ADCA0)
    P6DIR &= (~BIT1);// set P6.0 as input                 (P6.0作為輸入)
    //ADC12CTL0
    ADC12CTL0 = ADC12CTL0 & (~0xD000) | (0xDFFF);
    // ADC12SHT1x as 0000 for ADC12CLK 16 ADC clock cycle
    ADC12CTL0 = ADC12CTL0 & (~0x0D00) | (0xDFFF);;
    // ADC12SHT0x as 0000 for ADC12CLK 16 ADC clock cycle
    ADC12CTL0 = ADC12CTL0 | (BIT6);//ADC12REF2_5 = 1 參考電壓=2.5v
    ADC12CTL0 = ADC12CTL0 | (BIT5);//ADC12REFON = 1 參考電壓開啟
    ADC12CTL0 = ADC12CTL0 | (BIT4);//ADC12ON = 1    ADC 開啟
    ADC12CTL0 = ADC12CTL0 & (~0x000F);//disable interrupt disable ADC 關閉兩個中斷(Overflow interrupt & Conversion time overflow interrup) \
    //另外兩個是 ENC 跟SC  enable conversion & sample-and-conversion


    //ADC12CTL1
    ADC12CTL1 = ADC12CTL1 & (~0xF000);
    // ADC12CSTARTADDx 0000 for start convert from ADC12MEM0 以ADC12MEM0為轉換存放的位置(或連續模式的第一個位置)

    ADC12CTL1 = ADC12CTL1 & (~0x0C00);
    //ADC12SHSx as 00 for ADC12SC bit as s/h source
    //  ADC12_A sample-and-hold source select
    //        00b = ADC12SC bit

    ADC12CTL1 = ADC12CTL1 | (BIT9);
    //ADC12SHP as 1 pulse mode is from sampling timer
    //  ADC12_A sample-and-hold pulse-mode select.

    ADC12CTL1 = ADC12CTL1 & (~BIT8);
    //ADC12ISSH as 0 sample input is not inverted (訊號不反向)

    ADC12CTL1 = ADC12CTL1 & (~BIT7) & (~BIT6) & (~BIT5);
    //ADC12DIVx as 000 adc clock is divided by 1 預分頻為1

    ADC12CTL1 = ADC12CTL1 & (~BIT4) | (BIT3);
    //ADC12SSELx as 01 for sequence of channels(select ACLK)

    ADC12CTL1 = ADC12CTL1 & (~BIT2) & (~BIT1);
    //ADCCONSEQx as 00 for single channel single conv 單通道單次轉換

    ADC12CTL1 = ADC12CTL1 & (~BIT0); //清除忙碌flag


    //ADC12CTL2
    ADC12CTL2 = ADC12CTL2 & (~BIT8);
    //ADC12PDIV as 0 predivider by 1 //時鐘源預分頻

    ADC12CTL2 = ADC12CTL2 & (~BIT7); //片內溫度感測器"開啟"
    //ADC12TCOFF as 0 temp sensor on

    ADC12CTL2 = ADC12CTL2 | (BIT5) & (~BIT4);
    //ADC12RES as 10 resolution 12 bits //設定轉換結果解析度(10->12bit)

    ADC12CTL2 = ADC12CTL2 & (~BIT3);
    //ADC12DF as 0 bit unsigned binary //設定轉換結果為無號數

    ADC12CTL2 = ADC12CTL2 & (~BIT2);
    //ADC121SR as 0 buffer support up to 200kbps 設定取樣率趨近200kbps

    ADC12CTL2 = ADC12CTL2 | (BIT1);
    //ADC12REFOUT as 1 reference output on //參考輸出啟用
    ADC12CTL2 = ADC12CTL2 & (~BIT0);
    //ADC12REFBURST as 0 reference output continuously //參考緩衝器持續開啟

    //ADC12MCTL0
    ADC12MCTL0 = ADC12MCTL0 | (BIT7);
    //ADC12EOS as 1 end of sequence 決定最後一個轉換
    ADC12MCTL0 = ADC12MCTL0 & (~BIT6) & (~BIT5) | (BIT4);
    //ADC12SREFx as 001 reference selection as Vref+ to AVSS  //選擇參考變壓 001b = VR+ = VREF+ and V R- = AVSS
    ADC12MCTL0 = ADC12MCTL0 & (~BIT3) & (~BIT2) & (~BIT1) | (BIT0);
    //ADC12INCHx as 0000 select A0 //輸入通道選擇(A0)
    //ADC12IE
    ADC12IE = 0x0001;//interrupt enable A0 //中斷啟用


    __bis_SR_register(GIE); //開 global 中斷 Flag

    while(1){
        ADC12CTL0 |= (BIT4);//ADC12ON   開啟
        ADC12CTL0 |= (BIT0);//ADC12SC   開始取樣與轉換
        ADC12CTL0 |= (BIT1);//ADC12ENC  轉換開啟
        ADC12CTL0 &= (~BIT0);//ADC12SC  no sample and conversion start
        //這裡應該會跳去中斷(ADC12)
        float noz = (adcvalue - ADC_MIN) / (float)(ADC_MAX - ADC_MIN); //2^12 = 4096 (adcvalue - 479) / (4095 - 479)
        PWM_cycle((float)noz);

    }

}
//===========================================================
#pragma vector=ADC12_VECTOR
__interrupt void ADC12_FUNC(void)
{
    //switch( __even_in_range(P1IV, P1IV_P1IFG7 )) {
    switch(ADC12IV)
    {
        case 0x0006://ADC12IFG0
            adcvalue = ADC12MEM0;//adc data 讀值x8 or 向左靠其? <<3
            break;
        default:
            break;
    }
    ADC12IFG = 0x0000;//clear interrupt flag
}
//===========================================================

void PWM_cycle(float Per){
    int Percentage = (1040 * Per);
    TA0CCR1 = Percentage;
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

//===========================================================
// end of EV Bicycle Lab2 test01
//===========================================================
