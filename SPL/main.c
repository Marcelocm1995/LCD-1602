#include "main.h"
#include "lcd_1602_spl.h"
#include "stdio.h"

//prototipo das funcoes
void mDelay(uint32_t Delay);
void SetClk_Internal(void);

//variaveis globais
int i;
char strBuffer[64];


int main()
{
	//habilita o APB2
	__IO uint32_t tmpreg;
  SET_BIT(RCC->APB2ENR, RCC_APB2ENR_AFIOEN);
  tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_AFIOEN);
  (void)tmpreg;
	
	//habilita o APB1
  SET_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN);
  /* Delay after an RCC peripheral clock enabling */
  tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN);
  (void)tmpreg;
	
	//desabilita a interface jtag
	CLEAR_BIT(AFIO->MAPR,AFIO_MAPR_SWJ_CFG);
  SET_BIT(AFIO->MAPR, AFIO_MAPR_SWJ_CFG_DISABLE);
	
	//configura o NVIC (vetor de interrupcoes)
	NVIC_SetPriorityGrouping(((uint32_t)0x00000003));
	
	//configura o clock do uC
	SetClk_Internal();
	
	//habilita o LCD
	lcd_init();
	lcd_back_light(1);
	lcd_clear();
	lcd_cursor_mode(0,0);
			
	//escreve caracter por caracter
	lcd_send_char(0x4F);//O
	lcd_send_char(0x4C);//L
	lcd_send_char(0x41);//A
	lcd_send_char(0x20);// 
	lcd_send_char(0x32);//2
	lcd_send_char(0x30);//0
	lcd_send_char(0x32);//2
	lcd_send_char(0x31);//1
	
	//espera 2 segundos
	mDelay(2000);
	
	//limpa o LCD
	lcd_clear();

	
	while(1)
	{
		lcd_put_cur(0,0); 												 	 //posiciona o cursor
    sprintf(strBuffer, "Contador: %i", i);		 	 //monta a string
    lcd_send_string(strBuffer);									 //escreve a string no LCD
    i++;																				 //incrementa o valor do contador
		
		GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);   //acende o led da placa NUCLEO
		mDelay(500);																 //espera meio segundo
		
		GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET); //apaga o led da placa NUCLEO
		mDelay(500);																 //espera meio segundo
	}
}



void SetClk_Internal()
{
	//configura a latencia do clock
	MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, (0x2UL << (0U)));
	while(((uint32_t)(READ_BIT(FLASH->ACR, FLASH_ACR_LATENCY))) != (0x2UL << (0U)));
	
	//calibra o cristal interno
	RCC_AdjustHSICalibrationValue(16);
	
	//liga o cristal interno e espera estabilisar
	RCC_HSICmd(ENABLE);
	while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
	
	//liga a PLL para o cirstal interno e espera estabilisar
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, 16);
	RCC_PLLCmd(ENABLE);
	while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	
	//configura os prescaler's
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK2Config(RCC_HCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div2);
	
	//configura o clock do sistema e espera a confirmacao de configuracao
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while (RCC_GetSYSCLKSource() != 0x08);
	
	
	//configura o systick para base de 1ms
  SysTick->LOAD  = (uint32_t)((64000000 / 1000U) - 1UL);  /* set reload register */
  SysTick->VAL   = 0UL;                                       /* Load the SysTick Counter Value */
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_ENABLE_Msk;                   /* Enable the Systick Timer */
									 
	//informa o clock do sistema em 64Mhz							 
	SystemCoreClock = 64000000;	
}


//rotina de delay em milissegundos
void mDelay(uint32_t Delay)
{
  __IO uint32_t  tmp = SysTick->CTRL;  /* Clear the COUNTFLAG first */
  /* Add this code to indicate that local variable is not used */
  ((void)tmp);

  /* Add a period to guaranty minimum wait */
  if (Delay < 0xFFFFFFFFU)
  {
    Delay++;
  }

  while (Delay)
  {
    if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0U)
    {
      Delay--;
    }
  }
}
