#include <stm32f103xb.h>
extern void SystemClock_Config(void);
/*
 * File:SystemClock_Config.c
 * Description:
 * The SystemClock Setup
 * Author:Lichmore
 * 1.HSE enable
 */
void SystemClock_Config(void){
	RCC->CR |= RCC_CR_HSEON; //HSE enable
	while(!(RCC->CR & RCC_CR_HSERDY));//HSE ready


	RCC->CFGR |= RCC_CFGR_PLLXTPRE_HSE;//HSE divider for PLL entry
	RCC->CFGR |= RCC_CFGR_PLLSRC;//HSE clock as PLL entry source
	RCC->CFGR |= RCC_CFGR_PLLMULL9; //PLL mul ->72Mhz

	RCC->CR |= RCC_CR_PLLON;//PLL ON
	while(!(RCC->CR & RCC_CR_PLLRDY)); //PLL ready

	/* Enable Prefetch Buffer */
	FLASH->ACR |= FLASH_ACR_PRFTBE;

	/* Flash 2 wait state */
	FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
	FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2;

	RCC->CFGR |= RCC_CFGR_SW_PLL; //SYSCLK SRC is PLLCLK
	RCC->CR |= RCC_CR_CSSON; //CSS ON

	RCC->CFGR |= RCC_CFGR_HPRE_DIV1; //AHBprescalar 0xxx->no div 72mhz
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;  //APB1 div2
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;//APB2 div1



	/*
	 * 以下為測試用MCO 輸出
	 * 2:0[26 25 24]
	 * 0xx: No clock
	 * 100: System clock (SYSCLK) selected
	 * 101: HSI clock selected
	 * 110: HSE clock selected
	 * 111: PLL clock divided by 2 selected
	 * When the System Clock is selected to output to the MCO pin, make sure that this clock
	   does not exceed 50 MHz (the maximum IO speed).
	 *
	 *
	 */
	//RCC->CFGR |= RCC_CFGR_MCOSEL_HSE;
	//RCC->CFGR |= RCC_CFGR_MCOSEL_HSI;
	//RCC->CFGR |= RCC_CFGR_MCOSEL_PLL_DIV2;
	//RCC->CFGR |= RCC_CFGR_MCOSEL_SYSCLK;
}
