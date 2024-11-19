/* buzzer.h */

#ifndef ESET369_FINAL_SYSTEM_A_BUZZER_H
#define ESET369_FINAL_SYSTEM_A_BUZZER_H

#include <stdint.h>

#define eighth_note  (100)
#define quarter_note (eighth_note * 2)

/* TB1CCR0 = (SMCLK / desired frequency) - 1 [SMCLK=1MHz] */
typedef enum {
  C4  = 1911, /* C4  = 261.626 Hz */
  D4  = 1703, /* D4  = 293.665 Hz */
  E4  = 1517, /* E4  = 329.628 Hz */
  F4  = 1432, /* F4  = 349.228 Hz */
  G4  = 1275, /* G4  = 391.995 Hz */
} music_notes_t;

void buzzer_init(void);
void play_note(music_notes_t note, uint16_t duration_ms);
void play_song(void);

#endif /* ESET369_FINAL_SYSTEM_A_BUZZER_H */
