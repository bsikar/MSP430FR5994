#include <msp430.h>

/*
    Mode                            TA1CCR1 (AIN1)      TA1CCR2 (AIN2)
    Forward - fast spin             PWM (2000)          0
    Forward - slow spin             PWM (1000)          0
    Reverse - fast spin             0                   PWM (2000)
    Reverse - slow spin             0                   PWM (1000)
    Motor stop spin - slow decay    1                   1
    Motor stop spin - fast decay    0                   0
*/

unsigned int speed = 0;

int main(void) {
    WDTCTL   = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= BIT0;
    P1DIR |= BIT2 | BIT3;
    P1SEL1 &= ~(BIT2 | BIT3);
    P1SEL0 |= BIT2 | BIT3;

    P3DIR &= ~(BIT0 | BIT1);
    P3REN |= (BIT0 | BIT1);
    P3OUT |= (BIT0 | BIT1);

    P4DIR |= BIT7;
    P4OUT |= BIT7;

    P7DIR &= ~(BIT0 | BIT1);
    P7REN |= (BIT0 | BIT1);
    P7OUT |= (BIT0 | BIT1);

    TA1CTL = TASSEL_2 | MC_1 | TACLR;
    TA1CCR0 = 2000;
    TA1CCTL1 = OUTMOD_7;
    TA1CCTL2 = OUTMOD_7;

    ADC12CTL0  = ADC12SHT0_6 | ADC12ON;
    ADC12CTL1  = ADC12SHP;
    ADC12CTL2  = ADC12RES_2;
    ADC12MCTL0 = ADC12INCH_9;
    ADC12IER0  = ADC12IE0;

    __enable_interrupt();

    while(1) {
        ADC12CTL0 |= ADC12ENC | ADC12SC;

        if (!(P3IN & BIT0) && (P7IN & BIT0)) {        // Up ( Motor stop spin - slow decay )
            TA1CCR1 = 2000;
            TA1CCR2 = 2000;
        } else if (!(P3IN & BIT1) && (P7IN & BIT0)) { // Down ( Motor stop spin - fast decay )
            TA1CCR1 = 0;
            TA1CCR2 = 0;
        } else if ((P3IN & BIT1) && !(P7IN & BIT0)) { // Left ( Reverse )
            TA1CCR1 = 0;
            TA1CCR2 = speed;
        } else if ((P3IN & BIT0) && !(P7IN & BIT1)) { // Right ( Forward )
            TA1CCR1 = speed;
            TA1CCR2 = 0;
        }

        if (TA1CCR1 != TA1CCR2) {
            if (TA1CCR1 > TA1CCR2) {
                TA1CCR1 = speed;
            } else {
                TA1CCR2 = speed;
            }
        }

        __delay_cycles(50000);
    }
}

// if not blocked or black -> make fast (speed = 2000)
// if white -> make slow (speed = 500)
// adc_raw with black paper: 3920
// adc_raw with white paper: 990
// adc_raw with nothing:     3971
#pragma vector = ADC12_B_VECTOR
__interrupt void ADC12ISR(void) {
    if (ADC12IFGR0 & BIT0) {
        if (ADC12MEM0 > 2000) {
            speed = 2000;
        } else {
            speed = 500;
        }
    }
}
