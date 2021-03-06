/*
 * hw.h
 *
 *  Created on: 2022?~1??3??
 *      Author: Lichmore
 */

#ifndef HW_H_
#define HW_H_

#define Debounce 36500
#define pwm_MAX 3595
#define pwm_MIN 682
#define voltage_MAX 4096
#define voltage_MIN 0
#define current_MAX 4096
#define current_MIN 0
#define DUTY 1040


#define POS_LOW 2000
#define POS_HIGH 0 //1040*0.2

#define SECTION1 0,0,1,0,0,1
#define SECTION2 0,1,1,0,0,0
#define SECTION3 0,1,0,0,1,0
#define SECTION4 0,0,0,1,1,0
#define SECTION5 1,0,0,1,0,0
#define SECTION6 1,0,0,0,0,1

/*dont modify*/
//#define SECTION1 0,0,1,0,0,1
//#define SECTION2 0,1,1,0,0,0
//#define SECTION3 0,1,0,0,1,0
//#define SECTION4 0,0,0,1,1,0
//#define SECTION5 1,0,0,1,0,0
//#define SECTION6 1,0,0,0,0,1

#endif /* HW_H_ */
