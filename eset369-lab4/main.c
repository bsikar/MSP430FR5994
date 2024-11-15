#include <msp430.h> 


/* TA0CCR0 = (SMCLK / desired frequency) - 1 [SMCLK=1MHz] */
typedef enum music_notes_t {
    C4  = 1911, /* C4  = 261.626 Hz */
    C4S = 1803, /* C#4 = 277.183 Hz */
    D4  = 1703, /* D4  = 293.665 Hz */
    D4S = 1608, /* D#4 = 311.127 Hz */
    E4  = 1517, /* E4  = 329.628 Hz */
    F4  = 1432, /* F4  = 349.228 Hz */
    F4S = 1351, /* F#4 = 369.994 Hz */
    G4  = 1275, /* G4  = 391.995 Hz */
    G4S = 1203, /* G#4 = 415.305 Hz */
    A4  = 1136, /* A4  = 440.000 Hz */
    A4S = 1072, /* A#4 = 466.164 Hz */
    B4  = 1012, /* B4  = 493.883 Hz */
    C5  = 955,  /* C5  = 523.251 Hz */
    C5S = 901,  /* C#5 = 554.365 Hz */
    D5  = 851,  /* D5  = 587.330 Hz */
    D5S = 803,  /* D#5 = 622.254 Hz */
    E5  = 758,  /* E5  = 659.255 Hz */
    F5  = 716,  /* F5  = 698.457 Hz */
    F5S = 676,  /* F#5 = 739.989 Hz */
    G5  = 638,  /* G5  = 783.991 Hz */
    G5S = 602,  /* G#5 = 830.609 Hz */
    A5  = 568,  /* A5  = 880.000 Hz */
    A5S = 536,  /* A#5 = 932.328 Hz */
    B5  = 506,  /* B5  = 987.767 Hz */
    C6  = 477,  /* C6  = 1046.502 Hz */
    C6S = 450,  /* C#6 = 1108.731 Hz */
    D6  = 425,  /* D6  = 1174.659 Hz */
    D6S = 401,  /* D#6 = 1244.508 Hz */
    E6  = 379,  /* E6  = 1318.510 Hz */
    F6  = 358,  /* F6  = 1396.913 Hz */
    F6S = 338,  /* F#6 = 1479.978 Hz */
    G6  = 319,  /* G6  = 1567.982 Hz */
    G6S = 301,  /* G#6 = 1661.219 Hz */
    A6  = 284,  /* A6  = 1760.000 Hz */
    A6S = 268,  /* A#6 = 1864.656 Hz */
    B6  = 253   /* B6  = 1975.533 Hz */
} music_notes_t;

#define eighth_note  (100)
#define quarter_note (eighth_note * 2)
#define half_note    (quarter_note * 2)
#define whole_note   (half_note * 2)

#define TONGUE play_note(B6, 10) /* tongue noise */

/* Play a music note for a given time in ms */
void play_note(music_notes_t note, unsigned long duration_ms);

/**
 * main.c
 */
int main(void)
{
    WDTCTL   = WDTPW | WDTHOLD; /* stop watchdog timer */
    PM5CTL0 &= ~LOCKLPM5;       /* clear LOCKLPM5 */

    /* P6.0           (buzzer, output)
     * P5.0 P5.1      (output)
     * P3.0 P3.1 P3.2 (input)
     */
    P6DIR |= (BIT0);
    P6OUT &= ~(BIT0);
    P5DIR |= (BIT0 | BIT1);
    P5OUT |= (BIT0 | BIT1);
    P3DIR &= ~(BIT0 | BIT1 | BIT2 | BIT3);
    P3OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3);

    while (1) {
        P5OUT = BIT1;
        /* If button 4 is pressed, play C */
        if (!(P3IN & BIT0)) {
            play_note(C4, 200);
        }
        /* If button 5 is pressed, play D */
        if (!(P3IN & BIT1)) {
            play_note(D4, 200);
        }
        /* If button 6 is pressed, play E */
        if (!(P3IN & BIT2)) {
            play_note(E4, 200);
        }

        P5OUT = BIT0;
        /* If button 7 is pressed, play F */
        if (!(P3IN & BIT0)) {
            play_note(F4, 200);
        }

        /* If button 8 is pressed, play G */
        if (!(P3IN & BIT1)) {
            play_note(G4, 200);
        }
        /* If button 9 is pressed, bless your ears with a simple song */
        if (!(P3IN & BIT2)) {
            play_note(E4, quarter_note);
            TONGUE;
            play_note(E4, quarter_note);
            TONGUE;
            play_note(F4, quarter_note);
            TONGUE;
            play_note(G4, quarter_note);
            TONGUE;
            play_note(G4, quarter_note);
            TONGUE;
            play_note(F4, quarter_note);
            TONGUE;
            play_note(E4, quarter_note);
            TONGUE;
            play_note(D4, quarter_note);
            TONGUE;
            play_note(C4, quarter_note);
            TONGUE;
            play_note(C4, quarter_note);
            TONGUE;
            play_note(D4, quarter_note);
            TONGUE;
            play_note(E4, quarter_note);
            TONGUE;
            play_note(E4, quarter_note * 1.5);
            play_note(D4, eighth_note);
            play_note(D4, half_note);
        }
    }
}

void play_note(music_notes_t note, unsigned long duration_ms)
{
    unsigned int elapsed_ms = 0;

    /* Start the buzzer */
    TA1CCR0 = note;
    TA1CTL = TASSEL_2 | MC_1 | TACLR;

    while (elapsed_ms < duration_ms) {
        if ((TA1CTL & TAIFG) != 0) {
            P6OUT  ^= BIT0;   /* Toggle P6.0 */
            TA1CTL &= ~TAIFG; /* Clear the interrupt flag */
            elapsed_ms += 1;  /* Increment elapsed time */
        }
    }

    /* Stop the buzzer */
    TA1CTL &= ~(MC_1); /* Stop the timer */
    P6OUT &= ~BIT0;    /* Turn off the buzzer */
}
