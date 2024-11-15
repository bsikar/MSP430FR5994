#include <msp430.h> 


void LCD_command(unsigned char);
void LCD_write(unsigned char);
void LCD_init(void);

/**
 * main.c
 */
int main(void)
{
    unsigned int adc_raw;

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= 0x01;
    P4SEL1 |= BIT1;
    P4SEL0 |= BIT1;

    P3DIR |= 0xFF;
    P3OUT &= ~0xFF;
    P8DIR |= 0x0E;
    P8OUT &= ~0x0E;

    P5DIR |= 0x07;
    P5OUT &= ~0x07;

    ADC12CTL0 = ADC12SHT0_6 | ADC12ON;
    ADC12CTL1 = ADC12SHP;
    ADC12CTL2 = ADC12RES_2;
    ADC12MCTL0  = ADC12INCH_9;

    __delay_cycles(25000);
    LCD_init();
    LCD_command(0x80);
    LCD_write('B');
    LCD_write('r');
    LCD_write('i');
    LCD_write('g');
    LCD_write('h');
    LCD_write('t');
    LCD_write('o');
    LCD_write('n');

    while (1) {
        ADC12CTL0 |= ADC12ENC | ADC12SC;

        while ((ADC12IFGR0 & BIT0) == 0) {
            // do nothing
        }

        adc_raw = ADC12MEM0;
        P1OUT ^= 0x01;
        float converted_adc = adc_raw / 4095.0 * 3.3;

        LCD_command(0xC0);
        LCD_write('0' + converted_adc);
        LCD_write('.');
        int dec = (converted_adc - (int)converted_adc) * 1000; // 1000 for 3 dec places
        LCD_write('0' + (dec / 100) % 10);
        LCD_write('0' + (dec / 10) % 10);
        LCD_write('0' + dec % 10);

        if (converted_adc >= 3.0) {
            P5OUT = 0x00;
        } else if (converted_adc >= 2.0) {
            P5OUT = 0x04;
        } else if (converted_adc >= 1.0) {
            P5OUT = 0x06;
        } else {
            P5OUT = 0x07;
        }

        __delay_cycles(25000);
    }

    return 0;
}


void LCD_command(unsigned char in)
{
    P3OUT = in;
    P8OUT &= ~BIT3;
    P8OUT &= ~BIT2;
    P8OUT |= BIT1;
    __delay_cycles(200);
    P8OUT &= ~BIT1;
}

void LCD_write(unsigned char in)
{
    P3OUT = in;
    P8OUT |= BIT3;
    P8OUT &= ~BIT2;
    P8OUT |= BIT1;
    __delay_cycles(200);
    P8OUT &= ~BIT1;
}

void LCD_init(void)
{
    P8OUT &= ~BIT1;
    __delay_cycles(15000);
    LCD_command(0x30);
    __delay_cycles(300);
    LCD_command(0x30);
    __delay_cycles(300);
    LCD_command(0x30);
    __delay_cycles(300);
    LCD_command(0x38);
    LCD_command(0x10);
    LCD_command(0x0F);
    LCD_command(0x06);
    LCD_command(0x01);
    __delay_cycles(3000);
}
