#include <msp430.h> 

int main(void)
{
    int va, vb;
	WDTCTL   = WDTPW | WDTHOLD; /* stop watchdog timer */
	PM5CTL0 &= ~LOCKLPM5;       /* clear LOCKLPM5 */

	/* math */
	va = 27 + 30;
	vb = 27 - 30;

	/* set up LED with P1.2 and P1.3 (as outputs) */
	P1DIR |= BIT2 | BIT3;
	P1OUT |= BIT2 | BIT3;

	while (1) {
	    P1OUT |= BIT2;  /* turn on LED1 (P1.2) */
	    P1OUT &= ~BIT3; /* turn off LED2 (P1.3) */
	    __delay_cycles(250000);

	    P1OUT |= BIT3;  /* turn on LED2 (P1.3) */
	    P1OUT &= ~BIT2; /* turn off LED1 (P1.2) */
	    __delay_cycles(250000)
	}
}
