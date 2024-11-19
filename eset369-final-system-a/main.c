/* main.c */

#include <stdint.h>
#include <stdbool.h>
#include "msp430.h"
#include "lcd.h"
#include "timer.h"
#include "motor.h"
#include "servo.h"
#include "joystick.h"
#include "led.h"
#include "button.h"
#include "buzzer.h"
#include "uart.h"

int main(void) {
  WDTCTL   = WDTPW | WDTHOLD; /* Stop watchdog timer */
  PM5CTL0 &= ~LOCKLPM5;       /* Unlock GPIO pins */

  /* Initialize green LED */
  led_init();

  /* Initialize and LCD */
  lcd_init_gpio();
  lcd_init();

  /* Before anything is pressed, display name on second line */
  lcd_command(0xC0); /* Move to second line */
  lcd_write_s("Brighton");

  /* Joystick Y-Axis controls speed and direction of the DC Motor
   *
   * Center Position: Stop the Motor
   * +Y axis: Move one direction (Clockwise)
   * -Y axis: Move the other direction (Counter Clockwise)
   *
   * With the +/-Y Axis, the speed goes with the position of the axis
   *
   * Joystick X-Axis controls the angular position of the servo
   *
   * Center Position: Neutral position (0 degrees)
   * +X axis: Rotate the servo in one direction
   * -X axis: Rotate the servo in the other direction
   *
   * Angular position changes with the X-axis position
   * Minimum and maximum angles are -45 and 45 degrees, respectively
   */
  joystick_init_gpio();
  joystick_init_adc();

  /* motor + servo init */
  motor_init();
  servo_init();

  /* buttons init */
  button_init();

  /* buzzer init */
  buzzer_init();

  /* timer */
  __enable_interrupt(); /* Enable global interrupts */

  /* uart */
  uart_init();

  while (1) {
    /* system a */
    motor_set_speed(joystick_map_to_speed(joystick_get_y_axis()));
    servo_set_angle(joystick_map_to_degree(joystick_get_x_axis()));
    handle_led();
    handle_joystick_button();
    handle_button_a();
    handle_button_b();
    handle_button_c();

    /* system b */
    uart_send_system_b();

    __delay_cycles(100000);
  }
}
