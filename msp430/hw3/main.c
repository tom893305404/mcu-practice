#include <msp430.h>


/**lab3 時脈設定實驗
 * main.c
 *
 *
 * 輔助ACLK輸入源 要10Khz(XT1CLK)
 *
 * 副時脈SMCLK輸入源 12.5Mhz DCODIV
 *
 * 主時脈MCLK輸入源 25Mhz DCO
 *
 *
 *
 */
void SetVCoreUp(unsigned int level);
void CLKinit();
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	CLKinit();
	while(1){__no_operation(); }
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

//    UCSCTL0 = UCSCTL0 & (~BITC) & (~BITB) & (~BITA) & (~BIT9) & (~BIT8); //DCO as 00000, ((not sure,it set automatically normal
//
//    UCSCTL0 = UCSCTL0 & (~BIT7) & (~BIT6) & (~BIT5) & (~BIT4) & (~BIT3);//MOD as 00000,  ((not sure
//
//    UCSCTL1 = UCSCTL1 | (BIT6) & (~BIT5) & (~BIT4);//DCORSEL as 100, 25MHz //設定DCO要運行的頻率range 12.3~28.2Mhz
//
//    UCSCTL1 = UCSCTL1 & (~BIT0);//DISMOD as 0, modulate enable 設0啟用調變
//
//    UCSCTL2 = UCSCTL2 & (~BITE) & (~BITD) | (BITC);//set FLLD as 001, (div by 2) "D 除頻"D=2 000=/1 001=/2 010=/4 011=/8 100=16 other=/32
//
//    UCSCTL2 = UCSCTL2 & (~BIT9) | (BIT8) & (~BIT7) | (BIT6) | (BIT5) | (BIT4)
//            | (BIT3) | (BIT2) & (~BIT1) & (~BIT0);//FLLN N=380=(01 0111 1100) N除頻
//
//    UCSCTL3 = UCSCTL3 & (~BIT6) | (BIT5) | (BIT4);//SELREF as 010, (REFOCLK) FFL, ((FFL輸入為內部參考振盪器，低頻振盪器 32768Hz
//
//    UCSCTL3 = UCSCTL3 & (~BIT2) & (~BIT1) & (~BIT0);//FLLREFDIV as 000, (fFLLREFCLK/1) n=1(小n)

    UCSCTL0 |= 0x1080; UCSCTL0 &= 0xF087;
    UCSCTL1 |= 0x0060; UCSCTL1 &= 0xFFEE;
    UCSCTL2 |= 0x02FA; UCSCTL2 &= 0x8EFA;
    UCSCTL3 |= 0x0020; UCSCTL3 &= 0xFFA8;
    UCSCTL4 |= 0x0133; UCSCTL4 &= 0xF9BB;
    UCSCTL5 |= 0x0010; UCSCTL5 &= 0xF898;
    UCSCTL6 |= 0x01C0; UCSCTL6 &= 0x2FCC;
    UCSCTL7 |= 0x0000; UCSCTL7 &= 0xFFF0;

//    UCSCTL4 = UCSCTL4 & (~BITA) & (~BIT9) & (~BIT8);//SELA as 000, (XT1CLK) //輔助ACLK輸入源 要10Khz(XT1CLK)
//
//    UCSCTL4 = UCSCTL4 | (BIT6) & (~BIT5) & (~BIT4);//SELS as 100, (DIVDCOCLK) //副時脈SMCLK輸入源 12.5Mhz DCODIV
//
//    UCSCTL4 = UCSCTL4 & (~BIT2) | (BIT1) | (BIT0);//SELM as 011, (DCOCLK) //主時脈MCLK輸入源 25Mhz DCO

//    UCSCTL5 = UCSCTL5 | (BITA) & (~BIT9) | (BIT8);//DIVA as 000, (/1) //輔助時脈分頻//這裡記得改因為不知道外部晶體正當多少??
//
//    UCSCTL5 = UCSCTL5 & (~BIT6) & (~BIT5) & (~BIT4);//DIVS as 000, (/1) //副時脈分頻
//
//    UCSCTL5 = UCSCTL5 & (~BIT2) & (~BIT1) & (~BIT0);//DIVM as 000, (/1) //主時脈分頻
////===============================================================================================//
//    UCSCTL6 = UCSCTL6 & (~BITF) & (~BITE);//XT2DRIVE as 00, (4MHz-8MHz) //低電流消耗
//
//    UCSCTL6 = UCSCTL6 & (~BITC);//XT2BYPASS as 0 (crystal clock)//外部震盪晶體
//
//    UCSCTL6 = UCSCTL6 | (BIT8);//XT2OFF as 1 (XT2 is off) //XT2關閉不工作
//
//    UCSCTL6 = UCSCTL6 | (BIT7) | (BIT6);//XT1DRIVE as 11, (max drive ability)//最大驅動能力
//
//    UCSCTL6 = UCSCTL6 & (~BIT5);//XTs as 0 (XT1 low freq mode)//低速模式 XCAP啟用(why no set??)
//
//    UCSCTL6 = UCSCTL6 & (~BIT4);//XT1BYPASS as 0 (external crystal) //外部晶體震盪
//
//    UCSCTL6 = UCSCTL6 & (~BIT1);//SMCLKOFF as 0 (SMCLK is on) //開啟SMCLK
//
//    UCSCTL6 = UCSCTL6 & (~BIT0);//XT1OFF as 0 (XT1 is on)//XT1啟用
//
//    UCSCTL7 = UCSCTL7 & (~BIT3) & (~BIT2) & (~BIT1) & (~BIT0);//clear UCS flags//清除flag

    __bic_SR_register(SCG0);// Enable the FLL control loop //啟用FLL

    // (P1.0 is ACLK)
    P1SEL = P1SEL | (BIT0);        //port1 bit0 as 1 not digital io
    P1DIR = P1DIR | (BIT0);        //port1 bit0 as 1

    // (P2.2 is SMCLK)
    P2SEL = P2SEL | (BIT2);        //port2 bit2 as 1 not digital io
    P2DIR = P2DIR | (BIT2);        //port2 bit2 as 1

    // (P7.7 is MCLK)
    P7SEL = P7SEL | (BIT7);        //port7 bit7 as 1 not digital io
    P7DIR = P7DIR | (BIT7);        //port7 bit7 as 1 input

    // (P5.4 P5.5 is XT1)
    P5SEL = P5SEL | (BIT4);        //port5 bit4 as 1 not digital io
    P5SEL = P5SEL | (BIT5);        //port5 bit5 as 1 not digital io

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
