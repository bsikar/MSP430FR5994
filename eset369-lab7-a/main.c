#include <msp430.h>


// adc_raw with black paper: 3920
// adc_raw with white paper: 990
// adc_raw with nothing:     3971

 unsigned int adc_raw;
int main(void) {
    WDTCTL   = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR    |= 0x01;
    P4SEL1   |= BIT1;
    P4SEL0   |= BIT1;

    ADC12CTL0  = ADC12SHT0_6 | ADC12ON;
    ADC12CTL1  = ADC12SHP;
    ADC12CTL2  = ADC12RES_2;
    ADC12MCTL0 = ADC12INCH_9;
    ADC12IER0  = ADC12IE0;

    __enable_interrupt();

    while(1) {
        ADC12CTL0 |= ADC12ENC | ADC12SC;
        P1OUT     ^= 0x01;
        __delay_cycles(25000);
    }
}

#pragma vector = ADC12_B_VECTOR
__interrupt void ADC12ISR(void) {
    if (ADC12IFGR0 & BIT0) {
        adc_raw = ADC12MEM0;
    }
}
