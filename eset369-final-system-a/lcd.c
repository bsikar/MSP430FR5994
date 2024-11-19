/* lcd.c */

#include "lcd.h"
#include <stdint.h>
#include <stddef.h>
#include "msp430.h"

/* GPIO Versions */

void lcd_init_gpio(void)
{
  P8DIR |= 0x0E;  /* P8.1 to P8.3 */
  P8OUT &= ~0x0E; /* P8.1 to P8.3 */
  P3DIR |= 0xFF;  /* P3.0 to P3.7 */
  P3OUT &= ~0xFF; /* P3.0 to P3.7 */
}

void lcd_command(uint8_t cmd)
{
  P3OUT  = cmd;
  P8OUT &= ~BIT3;
  P8OUT &= ~BIT2;
  P8OUT |= BIT1;
  __delay_cycles(200);
  P8OUT &= ~BIT1;
}

void lcd_write(char ch)
{
  P3OUT  = ch;
  P8OUT |= BIT3;
  P8OUT &= ~BIT2;
  P8OUT |= BIT1;
  __delay_cycles(200);
  P8OUT &= ~BIT1;
}

void lcd_write_s(const char *str)
{
   while (*str) {
    lcd_write(*str++);
  }
}

void lcd_init(void)
{
  P8OUT &= ~BIT1;
  __delay_cycles(15000);
  lcd_command(0x30);
  __delay_cycles(300);
  lcd_command(0x30);
  __delay_cycles(300);
  lcd_command(0x30);
  __delay_cycles(300);
  lcd_command(0x38);
  lcd_command(0x10);
  lcd_command(0x0F);
  lcd_command(0x06);
  lcd_command(0x01);
  __delay_cycles(3000);
}

void lcd_display_int32(int32_t value)
{
  char    buffer[12]; /* Buffer to hold the string representation, including sign and null terminator */
  uint8_t index = 0;

  /* Handle the sign */
  if (value < 0) {
    buffer[index++] = '-'; /* Add negative sign */
    value = -value;        /* Make value positive for further processing */
  } else {
    buffer[index++] = '+'; /* Add positive sign */
  }

  /* Convert integer to string in reverse order */
  char temp[10]; /* Temporary buffer for reverse conversion */
  uint8_t temp_index = 0;

  do {
    temp[temp_index++] = (value % 10) + '0'; /* Extract digit and convert to character */
    value             /= 10;                 /* Remove the digit */
  } while (value > 0);

  /* Reverse the string */
  uint8_t i;
  for (i = 0; i < temp_index; i++) {
    buffer[index++] = temp[temp_index - i - 1];
  }

  /* Null-terminate the string */
  buffer[index] = '\0';

  /* Display the string on the LCD */
  lcd_write_s(buffer);
}
