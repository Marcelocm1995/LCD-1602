#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x.h"

extern void Delay(__IO uint32_t nCount);

#define LCD_D4_Pin GPIO_Pin_5
#define LCD_D4_GPIO_Port GPIOB
#define LCD_D5_Pin GPIO_Pin_4
#define LCD_D5_GPIO_Port GPIOB
#define LCD_D6_Pin GPIO_Pin_10
#define LCD_D6_GPIO_Port GPIOB
#define LCD_D7_Pin GPIO_Pin_8
#define LCD_D7_GPIO_Port GPIOA
#define LCD_BK_Pin GPIO_Pin_6
#define LCD_BK_GPIO_Port GPIOB
#define LCD_EN_Pin GPIO_Pin_7
#define LCD_EN_GPIO_Port GPIOC
#define LCD_RS_Pin GPIO_Pin_9
#define LCD_RS_GPIO_Port GPIOA
