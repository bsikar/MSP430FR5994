/* buzzer.c */

#include "buzzer.h"
#include <stdint.h>
#include "msp430.h"

void buzzer_init(void)
{
  P7DIR |= 0x01;  /* P7.0 (buzzer) */
  P7OUT &= ~0x01; /* P7.0 (buzzer) */
}

void play_note(music_notes_t note, uint16_t duration_ms)
{
  uint16_t elapsed_ms = 0;

  /* Start the buzzer using Timer B1 */
  TA2CCR0 = note;                    /* Set the period for the note */
  TA2CTL  = TBSSEL_2 | MC_1 | TBCLR; /* SMCLK, up mode, clear timer */

  while (elapsed_ms < duration_ms) {
    if ((TA2CTL & TAIFG) != 0) {
      P7OUT      ^= 0x01;   /* Toggle P7.0 */
      TA2CTL     &= ~TAIFG; /* Clear the interrupt flag */
      elapsed_ms += 1;      /* Increment elapsed time */
    }
  }

  /* Stop the buzzer */
  TA2CTL &= ~MC_1; /* Stop the timer */
  P7OUT  &= ~0x01; /* Turn off the buzzer */
}

void play_song(void)
{
  play_note(C4, quarter_note);
  play_note(D4, quarter_note);
  play_note(E4, quarter_note);
  play_note(F4, quarter_note);
  play_note(G4, quarter_note);
  play_note(F4, quarter_note);
  play_note(E4, quarter_note);
  play_note(D4, quarter_note);
  play_note(C4, quarter_note);
}
