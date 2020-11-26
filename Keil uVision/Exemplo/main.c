/* USER CODE BEGIN Includes */

#include "LCD_1602.h"
#include <string.h>

/* USER CODE END Includes */

/* USER CODE BEGIN PV */

int i;
char strBuffer[64];

/* USER CODE END PV */

/* USER CODE BEGIN 2 */
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
	
	HAL_Delay(1000);
	
	lcd_clear();
/* USER CODE END 2 */
  
  
/* USER CODE BEGIN WHILE */
	 lcd_put_cur(0,0);
     sprintf(strBuffer, "%i", i);
     lcd_send_string(strBuffer);
     i++;
	 HAL_Delay(1000);
		
/* USER CODE END WHILE */