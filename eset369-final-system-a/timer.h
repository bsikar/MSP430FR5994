/* timer.h */

#ifndef ESET369_FINAL_SYSTEM_A_TIMER_H
#define ESET369_FINAL_SYSTEM_A_TIMER_H

#include <stdint.h>
#include <stdbool.h>

/* 571.16mHz was measured with oscilloscope as the frequency:
 *
 * Ftoggle = ACLK_freq / (2 * (TA0CCR0 + 1))
 * -> ACLK_freq = 0.57116 * 2 * (32767 +1) = 37455Hz
 *
 * 32768 was the initial guess for the speed, but was wrong :shrug:
 */
#define ACLK_FREQUENCY (37455)
#define COUNTDOWN_MINS (1)
#define COUNTDOWN_SECS (15)

typedef enum {
  k_ready    = 0x01,
  k_running  = 0x02,
  k_paused   = 0x04,
  k_in_song  = 0x08,
  k_finished = 0x10, /* toggle green led only when the state is finished */
} countdown_state_t;

typedef struct {
  uint8_t           remaining_minutes;
  uint8_t           remaining_seconds;
  countdown_state_t countdown_state;
} countdown_t;

extern volatile countdown_t countdown;

void             timer_init(uint8_t min, uint8_t sec);
inline void      decrement_countdown(void);
inline void      timer_pause(void);
inline void      timer_start(void);
__interrupt void timer_interrupt(void);

#endif /* ESET369_FINAL_SYSTEM_A_TIMER_H */
