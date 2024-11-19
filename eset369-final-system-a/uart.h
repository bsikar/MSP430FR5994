/* uart.h */

#ifndef ESET369_FINAL_SYSTEM_B_UART_H
#define ESET369_FINAL_SYSTEM_B_UART_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
  uint16_t NADC_Xout;
  uint16_t NADC_Yout;
  uint8_t  *MM;
  uint8_t  *SS;
  bool button_state; /* 1: Pressed, 0: Released */
} system_b_data_t;

extern volatile system_b_data_t system_b_data;

void uart_init(void);
void uart_send_str(const char *str);
void uart_send_system_b(void);

#endif /* ESET369_FINAL_SYSTEM_B_UART_H */
