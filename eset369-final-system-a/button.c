/* button.c */

#include "button.h"
#include "msp430.h"
#include "timer.h"
#include "lcd.h"
#include "buzzer.h"
#include "uart.h"

void button_init(void)
{
  P6DIR &= ~0x03; /* P6.0 (Button A) P6.1 (Button B) */
  P6REN |= 0x03;  /* P6.0 (Button A) P6.1 (Button B) */
  P6OUT |= 0x03;  /* P6.0 (Button A) P6.1 (Button B) */

  P5DIR &= ~0x08; /* P5.3 (Button C) */
  P5REN |= 0x08;  /* P5.3 (Button C) */
  P5OUT |= 0x08;  /* P5.3 (Button C) */
}

bool is_button_pressed(char c)
{
  /* != 0 when configured as active high, == 0 for active low */
  switch (c) {
  case 'A':
  case 'a':
    return (P6IN & 0x01) == 0;
  case 'B':
  case 'b':
    return (P6IN & 0x02) == 0;
  case 'C':
  case 'c':
    return (P5IN & 0x08) == 0;
  default:
    return false;
  }
}

void handle_button_a(void)
{
  if (!is_button_pressed('a')) {
    system_b_data.button_state = false;
    return;
  }
  system_b_data.button_state = true;

  /* Start/Resume countdown */
  if (countdown.countdown_state == k_paused) {
    countdown.countdown_state = k_running;
  } else if (countdown.countdown_state == k_ready) {
    timer_start();
  }

  /* Display Counting on the second line */
  lcd_command(0x01); /* Clear the screen */
  __delay_cycles(3000);
  lcd_command(0xC0); /* Move to the second line */
  lcd_write_s("Counting");
}

void handle_button_b(void)
{
  if (!is_button_pressed('b')) {
    return;
  }

  /* Pause countdown */
  countdown.countdown_state = k_paused;

  /* Display Counting on the second line */
  lcd_command(0x01); /* Clear the screen */
  __delay_cycles(3000);
  lcd_command(0xC0); /* Move to the second line */
  lcd_write_s("Paused");
}

void handle_button_c(void)
{
  if (!is_button_pressed('c')) {
    return;
  }

  countdown.countdown_state |= k_in_song;
  /* Display Song name on first line */
  lcd_command(0x01); /* Clear the screen */
  __delay_cycles(3000);
  lcd_command(0x80); /* Move to first line */
  lcd_write_s("Do Re Mi");

  /* Display Song on second line */
  lcd_command(0xC0); /* Move to the second line */
  lcd_write_s("Song");

  /* Bless your ears with 'Do' 'Re' 'Mi' */
  play_song();
  lcd_command(0x01); /* Clear the screen */
  __delay_cycles(3000);
  countdown.countdown_state &= ~k_in_song;

  lcd_command(0xC0); /* Move to the second line */
  switch (countdown.countdown_state) {
  case k_paused:
    lcd_write_s("Paused");
    break;
  case k_ready:
    lcd_write_s("Ready");
    break;
  case k_finished:
    lcd_write_s("Time");
    break;
  case k_running:
    lcd_write_s("Counting");
    break;
  default:
    break;
  }
}
