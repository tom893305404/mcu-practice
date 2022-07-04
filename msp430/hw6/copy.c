/*#include <msp430.h>
//==========================================================
// EV Bicycle Lab2 test01
// This is simple ADC program for MSP430F5529
// ADC value control flash led rate
// (single channel single conversion)
//
// Created by Fuzen at 20190705
// Updated by Fuzen at 20191218 with comment
//===========================================================

void SetVCoreUp(unsigned int level);
void CLKinit();
void PWM_cycle(float Per);
volatile int Percentage=0;//PWM��



unsigned int adcvalue=10000;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   //stop watchdog timer

    //ADC ref setup ----------------------------------------
    REFCTL0 = REFCTL0 & (~BIT7);// set refmstr = 0
    P6SEL |= (BIT0);// p6.0 as peripheral module function(p6.0�~�]�ҥ�(ADCA0)
    P6DIR &= (~BIT0);// set P6.0 as input                 (P6.0�@����J)
    //ADC12CTL0
    ADC12CTL0 = ADC12CTL0 & (~0xD000) | (0xDFFF);
    // ADC12SHT1x as 0000 for ADC12CLK 16 ADC clock cycle
    ADC12CTL0 = ADC12CTL0 & (~0x0D00) | (0xDFFF);;
    // ADC12SHT0x as 0000 for ADC12CLK 16 ADC clock cycle
    ADC12CTL0 = ADC12CTL0 | (BIT6);//ADC12REF2_5 = 1 �Ѧҹq��=2.5v
    ADC12CTL0 = ADC12CTL0 | (BIT5);//ADC12REFON = 1 �Ѧҹq���}��
    ADC12CTL0 = ADC12CTL0 | (BIT4);//ADC12ON = 1    ADC �}��
    ADC12CTL0 = ADC12CTL0 & (~0x000F);//disable interrupt disable ADC ������Ӥ��_(Overflow interrupt & Conversion time overflow interrup) \
    //�t�~��ӬO ENC ��SC  enable conversion & sample-and-conversion


    //ADC12CTL1
    ADC12CTL1 = ADC12CTL1 & (~0xF000);
    // ADC12CSTARTADDx 0000 for start convert from ADC12MEM0 �HADC12MEM0���ഫ�s�񪺦�m(�γs��Ҧ����Ĥ@�Ӧ�m)

    ADC12CTL1 = ADC12CTL1 & (~0x0C00);
    //ADC12SHSx as 00 for ADC12SC bit as s/h source
    //  ADC12_A sample-and-hold source select
    //        00b = ADC12SC bit

    ADC12CTL1 = ADC12CTL1 | (BIT9);
    //ADC12SHP as 1 pulse mode is from sampling timer
    //  ADC12_A sample-and-hold pulse-mode select.

    ADC12CTL1 = ADC12CTL1 & (~BIT8);
    //ADC12ISSH as 0 sample input is not inverted (�T�����ϦV)

    ADC12CTL1 = ADC12CTL1 & (~BIT7) & (~BIT6) & (~BIT5);
    //ADC12DIVx as 000 adc clock is divided by 1 �w���W��1

    ADC12CTL1 = ADC12CTL1 & (~BIT4) | (BIT3);
    //ADC12SSELx as 01 for sequence of channels(select ACLK)

    ADC12CTL1 = ADC12CTL1 & (~BIT2) & (~BIT1);
    //ADCCONSEQx as 00 for single channel single conv ��q�D�榸�ഫ

    ADC12CTL1 = ADC12CTL1 & (~BIT0); //�M�����Lflag



    //ADC12CTL2
    ADC12CTL2 = ADC12CTL2 & (~BIT8);
    //ADC12PDIV as 0 predivider by 1 //�������w���W

    ADC12CTL2 = ADC12CTL2 & (~BIT7); //�����ū׷P����"�}��"
    //ADC12TCOFF as 0 temp sensor on

    ADC12CTL2 = ADC12CTL2 | (BIT5) & (~BIT4);
    //ADC12RES as 10 resolution 12 bits //�]�w�ഫ���G�ѪR��(10->12bit)

    ADC12CTL2 = ADC12CTL2 & (~BIT3);
    //ADC12DF as 0 bit unsigned binary //�]�w�ഫ���G���L����

    ADC12CTL2 = ADC12CTL2 & (~BIT2);
    //ADC121SR as 0 buffer support up to 200kbps �]�w���˲v�ͪ�200kbps

    ADC12CTL2 = ADC12CTL2 | (BIT1);
    //ADC12REFOUT as 1 reference output on //�Ѧҿ�X�ҥ�
    ADC12CTL2 = ADC12CTL2 & (~BIT0);
    //ADC12REFBURST as 0 reference output continuously //�Ѧҽw�ľ�����}��

    //ADC12MCTL0
    ADC12MCTL0 = ADC12MCTL0 | (BIT7);
    //ADC12EOS as 1 end of sequence �M�w�̫�@���ഫ
    ADC12MCTL0 = ADC12MCTL0 & (~BIT6) & (~BIT5) | (BIT4);
    //ADC12SREFx as 001 reference selection as Vref+ to AVSS  //��ܰѦ����� 001b = VR+ = VREF+ and V R- = AVSS
    ADC12MCTL0 = ADC12MCTL0 & (~BIT3) & (~BIT2) & (~BIT1) & (~BIT0);
    //ADC12INCHx as 0000 select A0 //��J�q�D���(A0)
    //ADC12IE
    ADC12IE = 0x0001;//interrupt enable A0 //���_�ҥ�

    // push button
    // (P4.1 is push button for adc sample)
    P4SEL &= (~BIT1);//port4 bit1 as digital io �@��IO
    P4DIR &= (~BIT1);//port4 bit1 as input  ��J
    P4REN |= (BIT1);//port4 bit1 as pull down input �ҥΤW�U�Թq��
    P4OUT &= (~BIT1);//port4 bit1 as pull down input    �ҥΤU�Թq��

    // Head Light ------------------------------------------�Y�O��l��
    // (P4.3 is a head light LED)
    P4SEL &= (~BIT3);//port4 bit3 as digital io
    P4DIR |= (BIT3);//port4 bit3 as output

    // (P4.0 is a switch for head light)
    P4SEL &= (~BIT0);//port4 bit0 as digital io
    P4DIR &= (~BIT0);//port4 bit0 as input
    P4REN |= (BIT0);//port4 bit0 as pull down input
    P4OUT &= (~BIT0);//port4 bit0 as pull down input

    // Brake Light ---------------------------------------- �٨��O��l��
    // (P8.1 is a emulated brake light)
    P8SEL &= (~BIT1);//port8 bit1 as digital io
    P8DIR |= (BIT1);//port8 bit1 as output
    P8OUT = P8OUT & (~BIT1);//brake light off

    __bis_SR_register(GIE); //�} global ���_ Flag

    volatile unsigned int counter;

    while(1)
    {
        P4OUT ^= (BIT3); //�Y�Otoggle

        if(P4IN & BIT0) //�Y�Oswitch
        {
        }
        else
        {
            P8OUT = P8OUT | (BIT1);//brake light on
            ADC12CTL0 |= (BIT4);//ADC12ON   �}��
            ADC12CTL0 |= (BIT0);//ADC12SC   �}�l���˻P�ഫ
            ADC12CTL0 |= (BIT1);//ADC12ENC  �ഫ�}��
            ADC12CTL0 &= (~BIT0);//ADC12SC  no sample and conversion start //�o�����ӷ|���h���_(ADC12)
            P8OUT = P8OUT & (~BIT1);//brake light off
        }

        if(P4IN & BIT1)
        {
            for(counter=2000; counter>0; counter--)
            {

            }
        }
        else
        {
            for(counter=adcvalue; counter>0; counter--)
            {

            }
        }
    }
}
//===========================================================
#pragma vector=ADC12_VECTOR
__interrupt void ADC12_FUNC(void)
{
    P8OUT = P8OUT | (BIT1);//brake light on
    //switch( __even_in_range(P1IV, P1IV_P1IFG7 )) {
    switch(ADC12IV)
    {
        case 0x0006://ADC12IFG0
            adcvalue = ADC12MEM0 << 3;//adc data Ū��x8
            break;
        default:
            break;
    }
    P8OUT = P8OUT & (~BIT1);//brake light off
    ADC12IFG = 0x0000;//clear interrupt flag
}
//===========================================================

void PWM_cycle(float Per){
    Percentage = (Per * 1040);
    TA0CCR1 = Percentage;
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
    __bis_SR_register(SCG0);// Disable the FLL control loop�������n����
    UCSCTL0 |= 0x1080; UCSCTL0 &= 0xF087;
    UCSCTL1 |= 0x0060; UCSCTL1 &= 0xFFEE;
    UCSCTL2 |= 0x02FA; UCSCTL2 &= 0x8EFA;
    UCSCTL3 |= 0x0020; UCSCTL3 &= 0xFFA8;
    UCSCTL4 |= 0x0133; UCSCTL4 &= 0xF9BB;
    UCSCTL5 |= 0x0010; UCSCTL5 &= 0xF898;
    UCSCTL6 |= 0x01C0; UCSCTL6 &= 0x2FCC;
    UCSCTL7 |= 0x0000; UCSCTL7 &= 0xFFF0;

    __bic_SR_register(SCG0);// Enable the FLL control loop //�ҥ�FLL


}

//===========================================================
// end of EV Bicycle Lab2 test01
//===========================================================
 *
*/
