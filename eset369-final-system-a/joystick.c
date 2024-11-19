/* joystick.c */

#include "joystick.h"
#include <stdint.h>
#include <stdbool.h>
#include "msp430.h"
#include "motor.h"
#include "timer.h"
#include "lcd.h"
#include "servo.h"
#include "uart.h"

void joystick_init_gpio(void)
{
  /* XXX: Why does 0x03 work, but 0x06 doesnt? 0b011 vs 0b110 */
  P4DIR &= ~0x03; /* P4.1 (Xout) P4.2 (Yout) */
  P4REN |= 0x03;  /* P4.1 (Xout) P4.2 (Yout) */
  P4OUT |= 0x03;  /* P4.1 (Xout) P4.2 (Yout) */

  P5DIR &= ~0x02; /* P5.1 (Sel) */
  P5REN |= 0x02;  /* P5.1 (Sel) */
  P5OUT |= 0x02;  /* P5.1 (Sel) */
}

void joystick_init_adc(void)
{
  ADC12CTL0  = ADC12SHT0_6 | ADC12ON | ADC12MSC;
  ADC12CTL1  = ADC12SHP | ADC12CONSEQ_1;
  ADC12CTL2  = ADC12RES_2;
  ADC12MCTL0 = ADC12INCH_9;
  ADC12MCTL1 = ADC12INCH_10 | ADC12EOS;
}

uint16_t joystick_get_x_axis(void)
{
  ADC12CTL0 |= ADC12ENC | ADC12SC;    /* Start ADC conversion */
  while ((ADC12IFGR0 & 0x01) == 0) {} /* Wait for conversion to complete */
  system_b_data.NADC_Xout = ADC12MEM0;
  return ADC12MEM0;                   /* Return X-axis ADC result */
}

uint16_t joystick_get_y_axis(void)
{
  ADC12CTL0 |= ADC12ENC | ADC12SC;    /* Start ADC conversion */
  while ((ADC12IFGR0 & 0x01) == 0) {} /* Wait for conversion to complete */
  system_b_data.NADC_Yout = ADC12MEM1;
  return ADC12MEM1;                   /* Return Y-axis ADC result */
}

int32_t joystick_map_to_speed(uint16_t adc_value)
{
  if (adc_value > (JOYSTICK_Y_CENTER - JOYSTICK_Y_TOLERANCE) &&
      adc_value < (JOYSTICK_Y_CENTER + JOYSTICK_Y_TOLERANCE)) {
    return 0; /* Neutral zone */
  }

  if (adc_value < JOYSTICK_Y_CENTER) {
    /* Map ADC value below center to MOTOR_SPEED_MIN to 0 */
    int32_t range        = JOYSTICK_Y_CENTER - JOYSTICK_Y_MIN;
    int32_t scaled_value = JOYSTICK_Y_CENTER - adc_value;
    return (int32_t)((int64_t)scaled_value * MOTOR_SPEED_MIN / range);
  } else {
    /* Map ADC value above center to 0 to MOTOR_SPEED_MAX */
    int32_t range        = JOYSTICK_Y_MAX - JOYSTICK_Y_CENTER;
    int32_t scaled_value = adc_value - JOYSTICK_Y_CENTER;
    return (int32_t)((int64_t)scaled_value * MOTOR_SPEED_MAX / range);
  }
}

int32_t joystick_map_to_degree(uint16_t adc_value)
{
  int32_t angle = 0;

  if (adc_value >= (JOYSTICK_X_CENTER - JOYSTICK_X_TOLERANCE) &&
      adc_value <= (JOYSTICK_X_CENTER + JOYSTICK_X_TOLERANCE)) {
    /* Within tolerance range, set angle to 0 */
    angle = 0;
  } else if (adc_value > JOYSTICK_X_CENTER) {
    /* Map joystick values from JOYSTICK_X_MIN to JOYSTICK_X_CENTER to negative angles */
    int32_t range        = JOYSTICK_X_CENTER - JOYSTICK_X_MIN;
    int32_t scaled_value = JOYSTICK_X_CENTER - adc_value;
    angle = (int32_t)(((int64_t)scaled_value * SERVO_ANGLE_MIN) / range);
  } else {
    /* Map joystick values from JOYSTICK_X_CENTER to JOYSTICK_X_MAX to positive angles */
    int32_t range        = JOYSTICK_X_MAX - JOYSTICK_X_CENTER;
    int32_t scaled_value = adc_value - JOYSTICK_X_CENTER;
    angle = (int32_t)(((int64_t)scaled_value * SERVO_ANGLE_MAX) / range);
  }

  return angle;
}

bool joystick_is_button_pressed(void)
{
  /* Read the input value of P5.1 (Sel button) */
  return (P5IN & 0x02) == 0;
}

void handle_joystick_button(void)
{
  if (joystick_is_button_pressed()) {
    /* Display on the second line */
    lcd_command(0x01); /* Clear the screen */
    __delay_cycles(3000);
    lcd_command(0xC0); /* Move to the second line */
    lcd_write_s("Ready");
    timer_init(COUNTDOWN_MINS, COUNTDOWN_SECS);
  }
}
