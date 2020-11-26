#include "mbed.h"
#include "meuLCD1602.h"

DigitalOut led(LED1);

int i;
char strBuffer[64];

int main()
{
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
    
    wait_ms(1000);
    lcd_clear();
       
    while(1) 
    {
        lcd_put_cur(0,0);
        sprintf(strBuffer, "%i", i);
        lcd_send_string(strBuffer);
        i++;
        wait_ms(1000);
    }
}
