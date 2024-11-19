/* servo.h */

#ifndef ESET369_FINAL_SYSTEM_A_SERVO_H
#define ESET369_FINAL_SYSTEM_A_SERVO_H

#include <stdint.h>

#define SERVO_ANGLE_MIN (-45)  /* Minimum servo angle in degrees */
#define SERVO_ANGLE_MAX (45)   /* Maximum servo angle in degrees */
#define SERVO_PULSE_MIN (1000) /* Minimum pulse width in microseconds */
#define SERVO_PULSE_MAX (2000) /* Maximum pulse width in microseconds */

void servo_init(void);
void servo_set_angle(int32_t angle);

#endif /* ESET369_FINAL_SYSTEM_A_SERVO_H */
