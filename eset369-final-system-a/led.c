/* led.c */

#include "led.h"
#include "msp430.h"
#include "timer.h"

void led_init(void)
{
  P1DIR |= 0x02;  /* P1.1 */
  P1OUT &= ~0x02; /* P1.1 */
}

void handle_led(void)
{
  if (countdown.countdown_state == k_finished) {
    P1OUT ^= 0x02; /* Toggle Green LED */
  }
}
