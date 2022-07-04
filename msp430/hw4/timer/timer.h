/*
 * timer.h
 *
 *  Created on: 202110る14ら
 *      Author: Lichmore
 */

#ifndef TIMER_H_
#define TIMER_H_

#define CLK_SRC_SEL_TAXCLK  0
#define CLK_SRC_SEL_ACLK    1
#define CLK_SRC_SEL_SMCLK   2
#define CLK_SRC_SEL_INCLK   3

#define CLK_MODE_STOP            0
#define CLK_MODE_CTC             1 //up
#define CLK_MODE_CONTINUOUS      2
#define CLK_MODE_UPDOWN          3



//==========================short cmd=====================================//
#define TA0CLR TA0CTL |= TACLR;
#define TA1CLR TA1CTL |= TACLR;
#define TA2CLR TA2CTL |= TACLR;
//interrupt enable
#define TA0INT_ENABLE TA0CLR |= TACLR;
#define TA1INT_ENABLE TA1CLR |= TACLR;
#define TA2INT_ENABLE TA2CLR |= TACLR;
//interrupt disable
#define TA0INT_DISABLE TA0CLR &= (~TACLR);
#define TA1INT_DISABLE TA1CLR &= (~TACLR);
#define TA2INT_DISABLE TA2CLR &= (~TACLR);
/*
 * function:timerCounterMode
 * 砞﹚璸计家Α 计 计 etc.
 * MC   mode    description
 * 00   Stop    ぃ璸计
 * 01   up      0~TAxCCR
 * 10   continuous  0~0xFFFF
 * 11   up/down     0~TAxCCR -> TAxCCR~0
 */
extern void timerCountMode(int timer,int mode);

/*
 * function:timerADiv
 * 砞﹚timerA箇だ繵竟
 */
extern void timerADiv(int timer,int divider,int dex);

/*
 * function:timerASourceSEL
 * 匡拒timerAx块方
 */
extern void timerASourceSEL(int timer, int clksrc);


#endif /* TIMER_H_ */
