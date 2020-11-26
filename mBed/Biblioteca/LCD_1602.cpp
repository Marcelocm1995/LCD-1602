#include "LCD_1602.h"

DigitalOut LCD_RS_pin(D8);
DigitalOut LCD_EN_Pin(D9);
DigitalOut LCD_D7_Pin(D7);
DigitalOut LCD_D6_Pin(D6);
DigitalOut LCD_D5_Pin(D5);
DigitalOut LCD_D4_Pin(D4);


void send_to_lcd (char data, int rs)
{
    LCD_RS_pin = rs;  // rs = 1 for data, rs=0 for command

    /* write the data to the respective pin */
    LCD_D7_Pin = (data>>3)&0x01;
    LCD_D6_Pin = (data>>2)&0x01;
    LCD_D5_Pin = (data>>1)&0x01;
    LCD_D4_Pin = (data>>0)&0x01;

    /* Toggle EN PIN to send the data
     * if the HCLK > 100 MHz, use the  20 us delay
     * if the LCD still doesn't work, increase the delay to 50, 80 or 100..
     */
    LCD_EN_Pin = 1;
    wait_ms(1);
    LCD_EN_Pin = 0;
    wait_ms(1);
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
    wait_ms(2);
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
    // 4 bit initialisation
    wait_ms(50);  // wait for >40ms
    lcd_send_cmd (0x00);
    wait_ms(5);  // wait for >4.1ms
    lcd_send_cmd (0x03);
    wait_ms(1);  // wait for >100us
    lcd_send_cmd (0x03);
    wait_ms(10);
    lcd_send_cmd (0x03);  // 4bit mode
    wait_ms(10);

  // dislay initialisation
    lcd_send_cmd (0x02); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
    wait_ms(1);
    lcd_send_cmd (0x02); //Display on/off control --> D=0,C=0, B=0  ---> display off
    wait_ms(1);
    lcd_send_cmd (0x08);  // clear display
    wait_ms(1);
    wait_ms(1);
    lcd_send_cmd (0x00); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
    wait_ms(1);
    lcd_send_cmd (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
    wait_ms(1);
    lcd_send_cmd (0x00); 
    wait_ms(1);
    lcd_send_cmd (0x06);
}

void lcd_send_string (char *str)
{
    while (*str) lcd_send_char (*str++);
}

