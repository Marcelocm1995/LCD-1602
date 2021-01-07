#include "lcd_1602_spl.h"

void send_to_lcd (char data, int rs)
{
	GPIO_WriteBit(LCD_RS_GPIO_Port, LCD_RS_Pin, rs);  // rs = 1 for data, rs=0 for command

	/* write the data to the respective pin */
	GPIO_WriteBit(LCD_D7_GPIO_Port, LCD_D7_Pin, ((data>>3)&0x01));
	GPIO_WriteBit(LCD_D6_GPIO_Port, LCD_D6_Pin, ((data>>2)&0x01));
	GPIO_WriteBit(LCD_D5_GPIO_Port, LCD_D5_Pin, ((data>>1)&0x01));
	GPIO_WriteBit(LCD_D4_GPIO_Port, LCD_D4_Pin, ((data>>0)&0x01));

	/* Toggle EN PIN to send the data
	 * if the HCLK > 100 MHz, use the  20 us delay
	 * if the LCD still doesn't work, increase the delay to 50, 80 or 100..
	 */
	GPIO_WriteBit(LCD_EN_GPIO_Port, LCD_EN_Pin, 1);
	Delay(1);
	GPIO_WriteBit(LCD_EN_GPIO_Port, LCD_EN_Pin, 0);
	Delay(1);
}

void lcd_cursor_mode(char cursor, char blink)
{
	if(cursor == 1)
	{
		lcd_send_cmd((0x0C)|0x02);
	}
	
	else
	{
		lcd_send_cmd((0x0C)&0x00);
	}
	
	if(blink == 1)
	{
		lcd_send_cmd((0x0C)|0x01);
	}
	
	else
	{
		lcd_send_cmd((0x0C)&0x00);
	}
}

void lcd_send_cmd (char cmd)
{
    char datatosend;

    /* send upper nibble first */
    datatosend = ((cmd>>4)&0x0f);
    send_to_lcd(datatosend,0);  // RS must be 0 while sending command

    /* send Lower Nibble */
    datatosend = ((cmd)&0x0f);
	send_to_lcd(datatosend, 0);
}

void lcd_send_char (char data)
{
	char datatosend;

	/* send higher nibble */
	datatosend = ((data>>4)&0x0f);
	send_to_lcd(datatosend, 1);  // rs =1 for sending data

	/* send Lower nibble */
	datatosend = ((data)&0x0f);
	send_to_lcd(datatosend, 1);
}

void lcd_clear (void)
{
	lcd_send_cmd(0x01);
	Delay(2);
}

void lcd_put_cur(int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }

    lcd_send_cmd (col);
}


void lcd_init (void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//Allow clock to GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//Configure pins
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//Allow clock to GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//Configure pins
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//Allow clock to GPIOC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//Configure pins
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	
	// 4 bit initialisation
	Delay(50);  // wait for >40ms
	lcd_send_cmd (0x00);
	Delay(5);  // wait for >4.1ms
	lcd_send_cmd (0x03);
	Delay(1);  // wait for >100us
	lcd_send_cmd (0x03);
	Delay(10);
	lcd_send_cmd (0x03);  // 4bit mode
	Delay(10);

  // dislay initialisation
	lcd_send_cmd (0x02); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	Delay(1);
	lcd_send_cmd (0x02); //Display on/off control --> D=0,C=0, B=0  ---> display off
	Delay(1);
	lcd_send_cmd (0x08);  // clear display
	Delay(1);
	Delay(1);
	lcd_send_cmd (0x00); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	Delay(1);
	lcd_send_cmd (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
	Delay(1);
	lcd_send_cmd (0x00); 
	Delay(1);
	lcd_send_cmd (0x06);
}

void lcd_send_string (char *str)
{
	while (*str) lcd_send_char (*str++);
}

