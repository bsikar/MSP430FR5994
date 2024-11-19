/* motor.h */

#ifndef ESET369_FINAL_SYSTEM_A_MOTOR_H
#define ESET369_FINAL_SYSTEM_A_MOTOR_H

#include <stdint.h>

#define MOTOR_SPEED_MIN (-2000) /* Minimum motor speed (full reverse) */
#define MOTOR_SPEED_MAX (2000)  /* Maximum motor speed (full forward) */

void motor_init(void);
void motor_set_speed(int32_t speed);

#endif /* ESET369_FINAL_SYSTEM_A_MOTOR_H */
