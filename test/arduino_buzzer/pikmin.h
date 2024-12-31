#include "pitches.h"

// Tempo settings
#define TEMPO         60000
#define QUARTER_NOTE  1

// Tempo values
#define MSPB                60000 / TEMPO
#define WHOLE_NOTE          4.00 * QUARTER_NOTE
#define HALF_NOTE           2.00 * QUARTER_NOTE
#define EIGHTH_NOTE         0.50 * QUARTER_NOTE
#define SIXTEENTH_NOTE      0.25 * QUARTER_NOTE
const float DOT(const float note) {return 1.5 * note;}

// Melody
#define NOTE_COUNT 4
int note_pitches[NOTE_COUNT] = {
  NOTE_DS6, NOTE_E6, NOTE_CS6, NOTE_C6
};
float note_durations[NOTE_COUNT] = {
  125, 500, 125, 1250
};