#include <xc.h>
#include <pic18f4550.h>

#pragma config PBADEN = OFF
#pragma config FOSC = HS
#pragma config PWRT = OFF
#pragma config BOR = OFF
#pragma config ICPRT = OFF
#pragma config LVP = OFF
#pragma config WDT = OFF
#pragma config DEBUG = OFF
#pragma config CPUDIV = OSC1_PLL2

#define _XTAL_FREQ 8000000

void send_cmd(unsigned char c)
{
    PORTB = 0X00;
    PORTB = PORTB | (c >> 4);
    PORTBbits.RB5 = 1;
    __delay_us(40);
    PORTBbits.RB5 = 0;
    __delay_us(40);
    PORTB = 0X00;
    PORTB = PORTB | (c & 0x0f);
    PORTBbits.RB5 = 1;
    __delay_us(40);
    PORTBbits.RB5 = 0;
    __delay_ms(2);
}
void send_data(unsigned char d)
{
    PORTB = 0B00010000;
    PORTB = PORTB | (d >> 4);
    PORTBbits.RB5 = 1;
    __delay_us(40);
    PORTBbits.RB5 = 0;
    __delay_us(40);
    PORTB = 0B00010000;
    PORTB = PORTB | (d & 0x0f);
    PORTBbits.RB5 = 1;
    __delay_us(40);
    PORTBbits.RB5 = 0;
    __delay_us(100);
}

void LCD_init()
{
    send_cmd(0x30);
    send_cmd(0x20);
    send_cmd(0x28);
    send_cmd(0x01);
    send_cmd(0x06);
    send_cmd(0x0c);
}

void main(void)
{
    TRISB = 0x00;
    PORTB = 0x00;
    TRISD = 0XFF;

    LCD_init();
    unsigned char position = 0x86;
    int old0 = 0;
    int old1 = 0;

    while (1)
    {
        if (PORTDbits.RD0 == 1 && old0 == 0)
        {
            position++;
        }
        old0 = PORTDbits.RD0;
        if (PORTDbits.RD1 == 1 && old1 == 0)
        {
            position--;
        }
        old1 = PORTDbits.RD1;
        send_cmd(position);
        send_data('h');
        send_data('e');
        send_data('l');
        send_data('l');
        send_data('o');
        __delay_ms(100);
        send_cmd(0x01);
    }
    return;
}
