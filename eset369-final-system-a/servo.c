/* servo.c */

#include "servo.h"
#include <stdint.h>
#include "msp430.h"

void servo_init(void)
{
  /* Configure P1.0 for TA0.1 PWM output */
  P1DIR  |= 0x01; /* Set P1.0 as output */
  P1SEL0 |= 0x01; /* Select TA0.1 function */
  P1SEL1 &= ~0x01;

  /* Configure Timer A0 for PWM */
  TA0CCR0  = 20000;                   /* Set timer period to 20ms (20,000us) */
  TA0CCTL1 = OUTMOD_7;                /* Reset/Set output mode */
  TA0CCR1  = SERVO_PULSE_MIN;         /* Set initial pulse width (minimum) */
  TA0CTL   = TASSEL_2 | MC_1 | TACLR; /* SMCLK, Up Mode, Clear TAR */

  servo_set_angle(0);
}

void servo_set_angle(int32_t angle)
{
  /* Clamp angle to valid range */
  if (angle < SERVO_ANGLE_MIN) {
    angle = SERVO_ANGLE_MIN;
  } else if (angle > SERVO_ANGLE_MAX) {
    angle = SERVO_ANGLE_MAX;
  }

  /* Map angle to pulse width (linear interpolation) */
  uint16_t pulse_width = SERVO_PULSE_MIN +
                         (angle - SERVO_ANGLE_MIN) * (SERVO_PULSE_MAX - SERVO_PULSE_MIN) /
                         (SERVO_ANGLE_MAX - SERVO_ANGLE_MIN);

  /* Set duty cycle */
  TA0CCR1 = pulse_width;
}
