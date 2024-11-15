#include <msp430.h> 

void LCD_command(unsigned char);
void LCD_write(unsigned char);
void LCD_init(void);

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    // 3 adc [0] is pot [1] is x [2] is y
    float adc[3] = {0, 0, 0};
    unsigned int converted_adc[3] = {0, 0, 0};

    // P8.3 P8.2 P8.1
    // P7.1 7.0
    // P6.0
    // [P5.2 -- input]
    // [P4.1 P4.2 P4.3 -- inputs]
    // P3.0 P3.1 P3.2 P3.3 P3.4 P3.5 P3.6 P3.7

    // OUTPUTS
    P8DIR |= BIT1 | BIT2 | BIT3;
    P8OUT &= ~(BIT1 | BIT2 | BIT3);
    P7DIR |= BIT0 | BIT1;
    P7OUT &= ~(BIT0 | BIT1);
    P6DIR |= BIT0;
    P6OUT &= ~BIT0;
    P3DIR |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7;
    P3OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    // INPUTS
    P5DIR &= ~BIT2;
    P5REN |= BIT2;
    P5OUT |= BIT2;
    P4DIR &= ~(BIT1 | BIT2 | BIT3);
    P4OUT |= BIT1 | BIT2 | BIT3;

    ADC12CTL0  = ADC12SHT0_6 | ADC12ON | ADC12MSC;
    ADC12CTL1  = ADC12SHP | ADC12CONSEQ_1;
    ADC12CTL2  = ADC12RES_2;
    ADC12MCTL0 = ADC12INCH_9;
    ADC12MCTL1 = ADC12INCH_10;
    ADC12MCTL2 = ADC12INCH_11 | ADC12EOS;

    __delay_cycles(25000);
    LCD_init();

    while (1) {
        ADC12CTL0 |= ADC12ENC | ADC12SC;

        while ((ADC12IFGR0 & BIT0) == 0) {
            // do nothing
        }

        // pot
        adc[0] = ADC12MEM0 / 4095.0 * 3.3;
        adc[1] = ADC12MEM1 / 4095.0 * 3.3;
        adc[2] = ADC12MEM2 / 4095.0 * 3.3;
        converted_adc[0] = (adc[0] - (int)adc[0]) * 1000;
        converted_adc[1] = (adc[1] - (int)adc[1]) * 1000;
        converted_adc[2] = (adc[2] - (int)adc[2]) * 1000;

        // P:0x000
        // X:0.001 Y:0.002
        LCD_command(0x80);
        LCD_write('P');
        LCD_write(':');
        LCD_write('0' + adc[0]);
        LCD_write('.');
        LCD_write('0' + (converted_adc[0] / 100) % 10);
        LCD_write('0' + (converted_adc[0] / 10) % 10);
        LCD_write('0' + converted_adc[0] % 10);


        LCD_command(0xC0);
        LCD_write('X');
        LCD_write(':');
        LCD_write('0' + adc[1]);
        LCD_write('.');
        LCD_write('0' + (converted_adc[1] / 100) % 10);
        LCD_write('0' + (converted_adc[1] / 10) % 10);
        LCD_write('0' + converted_adc[1] % 10);
        LCD_write(' ');
        LCD_write('Y');
        LCD_write(':');
        LCD_write('0' + adc[2]);
        LCD_write('.');
        LCD_write('0' + (converted_adc[2] / 100) % 10);
        LCD_write('0' + (converted_adc[2] / 10) % 10);
        LCD_write('0' + converted_adc[2] % 10);

        // part b (led)
        if (adc[2] > 1.7) {
            // if +y make red
            P7OUT = ~BIT1;  // red on
            P6OUT = ~0x0;   // blue off
        } else if (adc[2] < 1.5) {
            // if -y make blue
            P7OUT = ~0x0;   // red off
            P6OUT = ~BIT0;  // blue on
        } else if (adc[1] > 1.7) {
            // if +x make green
            P7OUT = ~BIT0;  // green on
        } else if (adc[1] < 1.5) {
            // if -x make yellow (red + green)
            P7OUT = ~(BIT0 | BIT1);  // red and green on
            P6OUT = ~0x0;          // blue off
        } else if ((P5IN & BIT2) != 0) {
            // if pressed make white (red + green + blue)
            P7OUT = BIT0 | BIT1;  // red and green on
            P6OUT = BIT0;         // blue on
        } else {
            // if in center make off
            P7OUT = 0x0;
            P6OUT = 0x0;
        }
    }
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
