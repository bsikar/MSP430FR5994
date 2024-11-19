/* motor.c */

#include "motor.h"
#include <stdint.h>
#include "msp430.h"

void motor_init(void)
{
  P1DIR  |= 0x0C;
  P1SEL1 &= ~0x0C;
  P1SEL0 |= 0x0C;
  P4DIR  |= 0x80;
  P4OUT  |= 0x80;

  /* Configure Timer A1 for PWM */
  TA1CCR0  = 2000;                    /* Set timer period (max speed 2000) */
  TA1CCTL1 = OUTMOD_7;                /* Reset/Set output mode for TA1.1 (AIN1) */
  TA1CCTL2 = OUTMOD_7;                /* Reset/Set output mode for TA1.2 (AIN2) */
  TA1CCR1  = 0;                       /* Initialize TA1.1 duty cycle to 0 (stop) */
  TA1CCR2  = 0;                       /* Initialize TA1.2 duty cycle to 0 (stop) */
  TA1CTL   = TASSEL_2 | MC_1 | TACLR; /* SMCLK, Up Mode, Clear TAR */
}

void motor_set_speed(int32_t speed)
{
  if (speed > 0) {
    /* Forward direction */
    TA1CCR1 = (uint16_t)speed; /* Set PWM duty cycle on TA1.1 (AIN1) */
    TA1CCR2 = 0;               /* Set TA1.2 (AIN2) to 0 */
  } else if (speed < 0) {
    /* Reverse direction */
    TA1CCR1 = 0;                  /* Set TA1.1 (AIN1) to 0 */
    TA1CCR2 = (uint16_t)(-speed); /* Set PWM duty cycle on TA1.2 (AIN2) */
  } else {
    /* Stop motor */
    TA1CCR1 = 0; /* Stop both PWM signals */
    TA1CCR2 = 0;
  }
}
