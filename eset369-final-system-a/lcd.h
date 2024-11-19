/* lcd.h */

#ifndef ESET369_FINAL_SYSTEM_A_LCD_H
#define ESET369_FINAL_SYSTEM_A_LCD_H

#include <stdint.h>

void lcd_init(void);
void lcd_init_gpio(void);
void lcd_command(uint8_t cmd);
void lcd_write(char ch);
void lcd_write_s(const char *str);
void lcd_display_int32(int32_t value);

#endif /* ESET369_FINAL_SYSTEM_A_LCD_H */
