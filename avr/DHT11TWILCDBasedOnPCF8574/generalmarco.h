/*
 * generalmarco.h
 *
 *  Created on: 2021年9月13日
 *      Author: Lichmore
 */

#ifndef GENERALMARCO_H_
#define GENERALMARCO_H_

#define SETBIT(x,n) ((x)|=1<<(n))
#define CLEARBIT(x,n) ((x)&=~(1<<(n)))
#define GETBIT(x,n) (((x)&1<<(n)) >> (n))
#define CHK_BIT(x, n) ( ((x) & (1<< (n)))!=0 )
#define FLIP_BIT(x, n) ( (x) ^= (1<< (n)) )

#endif /* GENERALMARCO_H_ */
