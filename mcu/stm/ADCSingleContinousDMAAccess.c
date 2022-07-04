
// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "diag/trace.h"
#include <stm32f103xb.h>


#include "../lichmore/adc/stm32_adc.h"
// ----------------------------------------------------------------------------
volatile uint16_t sample[10];

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"
void ADC_Configuration(void){
	/*GPIO Config*/
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	GPIOA->CRL &= ~( 0x03UL << GPIO_CRL_MODE0_Pos); //PA 0  as Analog mode
	GPIOA->CRL &= ~( 0x03UL << GPIO_CRL_CNF0_Pos);
	GPIOA->CRL &= ~( 0x03UL << GPIO_CRL_MODE1_Pos); //PA 1 as Analog mode
	GPIOA->CRL &= ~( 0x03UL << GPIO_CRL_CNF1_Pos);
	/*Clock config*/
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6; //adc clk as 12Mhz
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; // adc1 clock enable

	ADC1->CR1 &= ADC_CR1_AWDEN; //Analog watchdog disable on regular channels
	ADC1->CR1 &= ADC_CR1_JAWDEN;//Analog watchdog disable on injected channels

	ADC1->CR1 &= ADC_CR1_DUALMOD; // Independent mode.

	//ADC1->CR1 |= ADC_CR1_DISCNUM; //discontinous  mode channel count
	ADC1->CR1 &= ADC_CR1_JDISCEN;// Discontinous mode on injected channels
	ADC1->CR1 &= ADC_CR1_DISCEN;// Discontinous mode on  regular channels

	ADC1->CR1 &= ADC_CR1_JAUTO;//enable/disable automatic injected group conversion after regular group conversion.

	ADC1->CR1 &= ADC_CR1_AWDSGL; //Enable the watchdog on a single channel in scan mode

	ADC1->CR1 &= ADC_CR1_SCAN; // Scan mode

	ADC1->CR1 &= ADC_CR1_JEOCIE; // Interrupt enable for injected channels
	ADC1->CR1 &= ADC_CR1_AWDIE;//Analog watchdog interrupt enable
	ADC1->CR1 &= ADC_CR1_EOCIE;//Intettupt enable for EOC

	//ADC1->CR1 |= ADC_CR1_AWDCH;//Analog watchdog channel slecet bits

	/*------------->>>CR2<<------------*/
	ADC1->CR2 &= ADC_CR2_TSVREFE; //Temperature sensor and Vrefint enable

	//ADC1->CR2 &= ADC_CR2_SWSTART; //Start convertion of regular channels
	//ADC1->CR2 &= ADC_CR2_JSWSTART;//Start convertion of injected channels

	ADC1->CR2 &= ADC_CR2_EXTTRIG;//external trigger conversion mode for regular channels enable/disable
	//ADC1->CR2 &= ADC_CR2_EXTSEL;//select

	ADC1->CR2 &= ADC_CR2_JEXTTRIG;//external trigger conversion mode for injected channels enable/disable
	//ADC1->CR2 &= ADC_CR2_JEXTSEL;//select

	ADC1->CR2 &= ADC_CR2_ALIGN; //Align

	ADC1->CR2 |= ADC_CR2_DMA; // DMA enable/disable


	ADC1->CR2 |= ADC_CR2_CONT; //Continuous conversion(enable/disable) //連續模式


	ADC1->CR2 |= ADC_CR2_ADON; //enable A/D converter ON/OFF //上電


	ADC1->CR2 |= ADC_CR2_RSTCAL; //重整校準寄存器 //calibration(set by software cleared by hardware)
	while(ADC1->CR2 & ADC_CR2_RSTCAL);

	ADC1->CR2 |= ADC_CR2_CAL; //開啟校準 //Enable Calibration
	while(ADC1->CR2 & ADC_CR2_CAL);

/*
 * 注意: ADON 中原本為0時寫入1為喚醒，當原本其中為1時寫入1為開始轉換，
 * 當ADON以外其他位同時發生變化，則不觸發轉換，這是為了防止錯誤觸發。
 */
}

void DMA_setup(void){
	/*DMA 模塊時鐘*/
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;

	/*DMA setup*/
	DMA1_Channel1->CCR &= DMA_CCR_DIR; //read from peripheral
/*注意: 先寫入方向，否則其他寄存器的值會被重制*/

	DMA1_Channel1->CCR |= ( 3UL << DMA_CCR_PL_Pos); //very high pori
	DMA1_Channel1->CCR |= ( 1UL << DMA_CCR_MSIZE_Pos); //Msize as  16 bit
	DMA1_Channel1->CCR |= ( 1UL << DMA_CCR_PSIZE_Pos); //Psize as  16 bit
	DMA1_Channel1->CCR |= DMA_CCR_MINC;//記憶體位置自動增加模式
	DMA1_Channel1->CCR |= DMA_CCR_CIRC;//循環模式


	DMA1_Channel1->CNDTR = 10; //1筆資料

	DMA1_Channel1->CPAR = ((uint32_t)&ADC1->DR); //periphal address
	DMA1_Channel1->CMAR = ((uint32_t)&sample);//memory address

	DMA1_Channel1->CCR |= DMA_CCR_EN; //Enable


}
void adc_value_printf(void){
	uint16_t i;
	float voltage;
	for(i=0;i<10;i++){
		voltage = 3.3 * ( (float)sample[i] / (float)4096);
		trace_printf("%d:%d.%d%d ", i, (int)voltage, ((int)(voltage * 10) % 10), ((int)(voltage * 100) % 10));
	}
	trace_printf("\n");
}
int
main(int argc, char* argv[])
{
	NVIC_SetPriorityGrouping(0x07); // 不互相搶占中斷
	trace_printf("%d\n",SystemCoreClock); //print the SystemClock
	//adcSetup();
	ADC_Configuration();
	DMA_setup();

	{
		int i=0;
		for(i=0;i<10;i++)
			sample[i] = 200;
	}

	ADC1->SQR3 |= 0x00;//第一個轉換 ch0
	ADC1->SQR1 |= (0UL << ADC_SQR1_L_Pos);//有1個要轉換
	ADC1->CR2 |= ADC_CR2_ADON;//開始轉換

  while (1)
    {

	  while(DMA1->ISR & DMA_ISR_TCIF1){
		  adc_value_printf();
		  DMA1->IFCR |= DMA_IFCR_CGIF1;
		  DMA1->IFCR |= DMA_IFCR_CTCIF1;
		  DMA1->IFCR |= DMA_IFCR_CHTIF1;
	  }



    }
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

