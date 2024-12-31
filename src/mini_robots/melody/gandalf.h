#include "pitches.h"

// Tempo settings
#define TEMPO         130
#define QUARTER_NOTE  1.4

// Tempo values
#define MSPB                60000 / TEMPO
#define WHOLE_NOTE          4.00 * QUARTER_NOTE
#define HALF_NOTE           2.00 * QUARTER_NOTE
#define EIGHTH_NOTE         0.50 * QUARTER_NOTE
#define SIXTEENTH_NOTE      0.25 * QUARTER_NOTE
const float DOT(const float note) {return 1.5 * note;}

// Melody
#define NOTE_COUNT 26
int note_pitches[NOTE_COUNT] = {
  NOTE_B2, PAUSE, NOTE_B2, NOTE_B2, NOTE_B2, NOTE_A2, NOTE_B2, 
  NOTE_B2, PAUSE, NOTE_B2, NOTE_B2, NOTE_B2, NOTE_A2, NOTE_B2, 
  NOTE_B2, PAUSE, NOTE_D3, NOTE_B2, NOTE_A2,
  NOTE_A2, NOTE_G2, NOTE_E2, NOTE_E2, NOTE_FS2, NOTE_G2, NOTE_E2
};
float note_durations[NOTE_COUNT] = {
  QUARTER_NOTE, QUARTER_NOTE, EIGHTH_NOTE, SIXTEENTH_NOTE, SIXTEENTH_NOTE, SIXTEENTH_NOTE, DOT(EIGHTH_NOTE), 
  QUARTER_NOTE, QUARTER_NOTE, EIGHTH_NOTE, SIXTEENTH_NOTE, SIXTEENTH_NOTE, SIXTEENTH_NOTE, DOT(EIGHTH_NOTE), 
  QUARTER_NOTE, EIGHTH_NOTE, QUARTER_NOTE, QUARTER_NOTE, EIGHTH_NOTE,
  EIGHTH_NOTE, QUARTER_NOTE, EIGHTH_NOTE, EIGHTH_NOTE, EIGHTH_NOTE, EIGHTH_NOTE, EIGHTH_NOTE
};