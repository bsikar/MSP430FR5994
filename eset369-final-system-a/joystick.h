/* joystick.h */

#ifndef ESET369_FINAL_SYSTEM_A_JOYSTICK_H
#define ESET369_FINAL_SYSTEM_A_JOYSTICK_H

#include <stdint.h>
#include <stdbool.h>

#define JOYSTICK_X_MIN       (8)   /* Minimum ADC value for X-axis (~fully left) */
#define JOYSTICK_X_CENTER    (2217) /* Center ADC value for X-axis (~neutral position) */
#define JOYSTICK_X_MAX       (4095) /* Maximum ADC value for X-axis (~fully right) */
#define JOYSTICK_X_TOLERANCE (100)  /* Tolerance range around JOYSTICK_X_CENTER */

#define JOYSTICK_Y_MIN       (1)   /* Minimum ADC value for Y-axis (~fully up) */
#define JOYSTICK_Y_CENTER    (2006) /* Center ADC value for Y-axis (~neutral position) */
#define JOYSTICK_Y_MAX       (4095) /* Maximum ADC value for Y-axis (~fully down) */
#define JOYSTICK_Y_TOLERANCE (100)  /* Tolerance range around JOYSTICK_Y_CENTER */

void     joystick_init_gpio(void);
void     joystick_init_adc(void);
uint16_t joystick_get_x_axis(void);
uint16_t joystick_get_y_axis(void);
int32_t  joystick_map_to_speed(uint16_t adc_value);
int32_t  joystick_map_to_degree(uint16_t adc_value);
bool     joystick_is_button_pressed(void);
void     handle_joystick_button(void);

#endif /* ESET369_FINAL_SYSTEM_A_JOYSTICK_H */
