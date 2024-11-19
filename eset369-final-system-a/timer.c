/* timer.c */

#include "timer.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "msp430.h"
#include "lcd.h"

volatile countdown_t countdown;

void timer_init(uint8_t min, uint8_t sec)
{
  countdown = (countdown_t){min, sec, k_ready};
  TB0CTL    = TBSSEL_1 | MC_1 | TBCLR; /* ACLK, Up mode, Clear timer */
  TB0CCR0   = ACLK_FREQUENCY - 1;      /* Set Compare Register for 1-second interval */
  TB0CCTL0  = CCIE;                    /* Enable interrupt on timer overflow */
}

inline void timer_start(void)
{
  countdown.countdown_state = k_running;
}

inline void timer_pause(void)
{
  countdown.countdown_state = k_paused;
}

inline void decrement_countdown(void)
{
  if (countdown.remaining_seconds == 0 &&
      countdown.remaining_minutes > 0) {
    countdown.remaining_minutes -= 1;
    countdown.remaining_seconds  = 59;
  } else {
    countdown.remaining_seconds -= 1;
  }
}

#pragma vector = TIMER0_B0_VECTOR
__interrupt void timer_interrupt(void)
{
  if (countdown.remaining_minutes == 0 &&
      countdown.remaining_seconds == 0) {
    /* Display on the second line */
    lcd_command(0x01); /* Clear the screen */
    __delay_cycles(3000);
    lcd_command(0xC0); /* Move to the second line */
    lcd_write_s("Time");

    /* Start Blinking Green LED */
    countdown.countdown_state = k_finished;
  } else {
    if ((countdown.countdown_state & k_in_song) == 0) {
      /* Display on the first line */
      lcd_command(0x80); /* Move to the top line */
      lcd_write('0' + (countdown.remaining_minutes / 10)); /* Tens place of minutes */
      lcd_write('0' + (countdown.remaining_minutes % 10)); /* Units place of minutes */
      lcd_write(':');                                      /* Separator */
      lcd_write('0' + (countdown.remaining_seconds / 10)); /* Tens place of seconds */
      lcd_write('0' + (countdown.remaining_seconds % 10)); /* Units place of seconds */
    }

    if ((countdown.countdown_state & k_running) == k_running) {
      decrement_countdown();
    }
  }
}
