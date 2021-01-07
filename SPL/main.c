#include "main.h"
#include "lcd_1602_spl.h"
#include "stdio.h"

RCC_ClocksTypeDef ClksFreq;

void Delay(__IO uint32_t nCount);
void NMI_Handler(void);
void SetClk(uint32_t PLLMul);

int i;
char strBuffer[64];

int main()
{
	//==================System Clock Init==================
	SetClk(2);
	
	lcd_init();
	lcd_clear();
	lcd_cursor_mode(0,0);
			
	lcd_send_char(0x4F);//O
	lcd_send_char(0x4C);//L
	lcd_send_char(0x41);//A
	lcd_send_char(0x20);// 
	lcd_send_char(0x32);//2
	lcd_send_char(0x30);//0
	lcd_send_char(0x32);//2
	lcd_send_char(0x30);//0
	
	Delay(1000);
	
	lcd_clear();

	
	while(1)
	{
		lcd_put_cur(0,0);
    sprintf(strBuffer, "%i", i);
    lcd_send_string(strBuffer);
    i++;
		Delay(1000);
		
	}
}

void NMI_Handler()
{
	//Clear CSS interrupt flag
	RCC->CIR |= RCC_CIR_CSSC;
	//Wait couple of time, if it recovers
	//feasible to restart again
	Delay(100);
	//Try to start HSE
	RCC_HSEConfig(RCC_HSE_ON);
	//Delay to start crystal
	Delay(1);
	if (RCC_WaitForHSEStartUp() == SUCCESS)
	{
		//If starts - set HSE as system clock source
		RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);
		//Stop HSI
		RCC_HSICmd(DISABLE);
	}
	else GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	//If crystal does not started - stay on HSI
	//Light up LED on GPIO_Pin_8
}


void SetClk(uint32_t PLLMul) {
      RCC_DeInit();
	  //Run HSE
	  RCC_HSEConfig(RCC_HSE_ON);
	  //Wait until HSE starts up
	  while(RCC_WaitForHSEStartUp() != SUCCESS);
	  //Turn on Clock Security System
	  RCC->CR |= RCC_CR_CSSON;
	  //Make HSE as system main clock source
	  //RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);
	  //Power off HSI to reduce energy
	  RCC_HSICmd(DISABLE);

      // PLL provides frequency multiplier of (HSI/2) i.e. 4MHz x ...
      RCC_PLLConfig(RCC_PLLSource_HSE_Div2, PLLMul);
      // Enable PLL
      RCC_PLLCmd(ENABLE);
      // Wait till PLL is ready
      while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
      // Select PLL as system clock source
      RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
      // Wait till PLL is used as system clock source
      while (RCC_GetSYSCLKSource() != 0x08);
      // AHB, AP2 and AP1 clock are necessary for the peripherals to function
      // HCLK for AHB = SYSCLK (max is SYSCLK, up to 72MHz)
      RCC_HCLKConfig(RCC_SYSCLK_Div1);
      // PCLK2 for APB2 = HCLK (max is SYSCLK, up to 72MHz)
      RCC_PCLK2Config(RCC_HCLK_Div1);
      // PCLK1 for APB1 = HCLK (HCLK <= 36MHz)
      RCC_PCLK1Config(RCC_HCLK_Div1);

      RCC_GetClocksFreq(&ClksFreq); // update SYSCLK, HCLK, PCLK1 and PCLK2 in ClksFreq
}


//Dumbest delay function
void Delay(__IO uint32_t nCount)
{
	uint32_t i = 0;
	for (; nCount != 0; i++)
	{
		if (i == 1000)
		{
			i = 0;
			nCount--;
		}
	}
}
