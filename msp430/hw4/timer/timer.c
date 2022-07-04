/*
 * timer.c
 * ez way to set timer
 *  Created on: 2021年10月14日
 *      Author: Lichmore
 */
#include <msp430.h>
#include <stdlib.h>
#include "timer.h"


//===============end of short cmd=======================//
//TCNTR == TAxR
/*Registers
    00h TAxCTL       Timer_Ax        Control
    02h TAxCCTL0     Timer_Ax        Capture/Compare Control 0
    04h TAxCCTL1     Timer_Ax        Capture/Compare Control 1
    06h TAxCCTL2     Timer_Ax        Capture/Compare Control 2
    08h TAxCCTL3     Timer_Ax        Capture/Compare Control 3
    0Ah TAxCCTL4     Timer_Ax        Capture/Compare Control 4
    0Ch TAxCCTL5     Timer_Ax        Capture/Compare Control 5
    0Eh TAxCCTL6     Timer_Ax        Capture/Compare Control 6
    10h TAxR         Timer_Ax        Counter
    12h TAxCCR0      Timer_Ax        Capture/Compare 0
    14h TAxCCR1      Timer_Ax        Capture/Compare 1
    16h TAxCCR2      Timer_Ax        Capture/Compare 2
    18h TAxCCR3      Timer_Ax        Capture/Compare 3
    1Ah TAxCCR4      Timer_Ax        Capture/Compare 4
    1Ch TAxCCR5      Timer_Ax        Capture/Compare 5
    1Eh TAxCCR6      Timer_Ax        Capture/Compare 6
    2Eh TAxIV        Timer_Ax        Interrupt Vector
    20h TAxEX0       Timer_Ax        Expansion 0
*/
/*
 * function:timerASourceSEL
 * 選擇timerAx輸入源
 */
void timerASourceSEL(int timer, int clksrc){
    volatile unsigned int *timerP = NULL;
    switch(timer){
    case 0://timerA0
        timerP = &TA0CTL;
        break;
    case 1://timerA1
        timerP = &TA1CTL;
        break;
    case 2://timerA2
        timerP = &TA2CTL;
        break;
    }
    switch(clksrc){
    case CLK_SRC_SEL_TAXCLK: //00
        *(timerP) = *(timerP) & (~BIT9) & (~BIT8);
        break;
    case CLK_SRC_SEL_ACLK: //01
        *(timerP) = *(timerP) & (~BIT9) | (BIT8);
        break;
    case CLK_SRC_SEL_SMCLK: //10
        *(timerP) = *(timerP) | (BIT9) & (~BIT8);
        break;
    case CLK_SRC_SEL_INCLK: //11
        *(timerP) = *(timerP) | (BIT9) | (BIT8);
        break;
    }
}

/*
 * function:timerADiv
 * 設定timerA預分頻器
 */
void timerADiv(int timer,int divider,int dex){
    volatile unsigned int *timerP = NULL;
    volatile unsigned int *TAxEx0P = NULL;
    switch(timer){
    case 0://timerA0
        timerP = &TA0CTL;
        TAxEx0P = &TA0EX0;
        break;
    case 1://timerA1
        timerP = &TA1CTL;
        TAxEx0P = &TA1EX0;
        break;
    case 2://timerA2
        timerP = &TA2CTL;
        TAxEx0P = &TA2EX0;
        break;
    }
    switch(divider){ //預分頻 1 2 4 8
    case 1:
        *(timerP) = *(timerP) & (~BIT7) & (~BIT6); //00
        break;
    case 2:
        *(timerP) = *(timerP) & (~BIT7) | (BIT6);//01
        break;
    case 4:
        *(timerP) = *(timerP) | (BIT7) & (~BIT6);//10
        break;
    case 8:
        *(timerP) = *(timerP) | (BIT7) | (BIT6);//11
        break;
    }

    switch(dex){ //預分頻  1 2 3 4 5 6 7 8
        case 1:
            *(TAxEx0P) = 0x0000;
            break;
        case 2:
            *(TAxEx0P) = 0x0001;
            break;
        case 3:
            *(TAxEx0P) = 0x0002;
            break;
        case 4:
            *(TAxEx0P) = 0x0003;
            break;
        case 5:
            *(TAxEx0P) = 0x0004;
            break;
        case 6:
            *(TAxEx0P) = 0x0005;
            break;
        case 7:
            *(TAxEx0P) = 0x0006;
            break;
        case 8:
            *(TAxEx0P) = 0x0007;
            break;
        }
}

/*
 * function:timerCounterMode
 * 設定計數模式 上數 下數 etc.
 * MC   mode    description
 * 00   Stop    不計數
 * 01   up      0~TAxCCR
 * 10   continuous  0~0xFFFF
 * 11   up/down     0~TAxCCR -> TAxCCR~0
 */
void timerCountMode(int timer,int mode){
    volatile unsigned int *timerP = NULL;
        switch(timer){
        case 0://timerA0
            timerP = &TA0CTL;
            break;
        case 1://timerA1
            timerP = &TA1CTL;
            break;
        case 2://timerA2
            timerP = &TA2CTL;
            break;
        }
        switch(mode){
        case CLK_MODE_STOP:
            *(timerP) = *(timerP) & (~BIT5) & (~BIT4);//00
            break;
        case CLK_MODE_CTC:
            *(timerP) = *(timerP) & (~BIT5) | (BIT4);//01
            break;
        case CLK_MODE_CONTINUOUS:
            *(timerP) = *(timerP) | (BIT5) & (~BIT4);//10
            break;
        case CLK_MODE_UPDOWN:
            *(timerP) = *(timerP) | (BIT5) | (BIT4);//11
            break;
        }
}

//==============================Capture/Compare Registers=========================//


//=============================Capture mode=======================================//


/*
 * function:TAxCaptureTriggerMode()
 *輸入捕捉模式的觸發方式 沒有00 上升01 下降10 上升下降11 CM (15 14bit)
 */
void TAxCaptureNMode(int timer,int number,int mode){

}

/*
 * function:TAxCaptureInputSEL
 * (13 12 bit)
 */
void TAxCaptureInputSEL();

/*
 * function:TAxCaptureSynSEL
 * 同步非同步輸入捕捉設定(bit 11)
 */
void TAxCaptureSynSEL();

//10bit SCCI沒做


/*
 * function:TAxCaptureEn
 * 輸入捕捉功能開關8 bit
 */
void TAxCaptureEn();


//============================end of capture mode==============================//
//bit 7-5
void TAxOutputNMode();

//4 CCIE interrupt enable/disable
//3 CCI R only,the selected input can read by this bit
//2 OUT output ctl
//1 COV capture ovf need clear by soft
//0 CCIFG interrup flag
