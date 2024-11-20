/* uart.c */
#include "uart.h"
#include <stdio.h>
#include "msp430.h"
#include "timer.h"

volatile system_b_data_t system_b_data = {0, 0,
                                          &(countdown.remaining_minutes),
                                          &(countdown.remaining_seconds),
                                          0};

void uart_init(void)
{
  /* Configure GPIO for UART */
  P2SEL1 |= BIT0 | BIT1;    /* P2.0 = UCA0TXD, P2.1 = UCA0RXD */
  P2SEL0 &= ~(BIT0 | BIT1); /* Clear P2.0 and P2.1 settings */

  /* Configure UART */
  UCA0CTLW0 = UCSWRST;        /* Put eUSCI in reset state */
  UCA0CTLW0 |= UCSSEL__SMCLK; /* Use SMCLK as clock source */

  /* Baud Rate Settings for 9600 bps with 1 MHz SMCLK */
  UCA0BRW    = 6;           /* Clock prescaler */
  UCA0MCTLW  = UCOS16 |     /* Enable oversampling mode */
               (8 << 4) |   /* First-stage modulation (BRF = 8) */
               (0x20 << 8); /* Second-stage modulation (BRS = 0x20) */

  UCA0CTLW0 &= ~UCSWRST; /* Release eUSCI from reset state */
  UCA0IE    |= UCRXIE;   /* Enable RX interrupt */
}

void uart_send_str(const char *data)
{
  while (*data) {
    while (!(UCA0IFG & UCTXIFG)); /* Wait for TX buffer to be ready */
    UCA0TXBUF = *(data++);        /* Transmit next character */
  }
}

void uart_send_system_b(void)
{
  char buffer[32]; /* Buffer for the formatted string */
  char mm_padded[3]; /* Padded MM (2 digits) */
  char ss_padded[3]; /* Padded SS (2 digits) */

  /* Pad MM with zero if needed */
  snprintf(mm_padded, sizeof(mm_padded), "%d", *(system_b_data.MM));
  if (*(system_b_data.MM) < 10) {
    mm_padded[1] = mm_padded[0];
    mm_padded[0] = '0';
    mm_padded[2] = '\0';
  }

  /* Pad SS with zero if needed */
  snprintf(ss_padded, sizeof(ss_padded), "%d", *(system_b_data.SS));
  if (*(system_b_data.SS) < 10) {
    ss_padded[1] = ss_padded[0];
    ss_padded[0] = '0';
    ss_padded[2] = '\0';
  }

  /* Format data into the buffer */
  int length = snprintf(buffer, sizeof(buffer),
                        "%d,%d,%s,%s,%d\r\n",
                        system_b_data.NADC_Xout,
                        system_b_data.NADC_Yout,
                        mm_padded,
                        ss_padded,
                        system_b_data.button_state);

  /* Send the formatted string if valid */
  if (length > 0 && length < sizeof(buffer)) {
    uart_send_str(buffer);
  }
}

