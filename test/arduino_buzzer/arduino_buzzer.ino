#include "gandalf.h"

#define PLAY_COUNT 1   // The number of times the melody will be played (-1 is infinite)

void Play_Melody();
void Start_Song();
void Stop_Song();

int remaining;

void setup() {
  Start_Song();
}

void loop() {
  while(remaining != 0) {
    Play_Melody();
    remaining--;
  }
}

void Play_Melody() {
  // Iterate over the notes of the melody
  for (int i = 0; i < NOTE_COUNT; i++) {

    // Calculate the note duration
    int note_duration = MSPB * note_durations[i];
    tone(8, note_pitches[i], note_duration);

    // To distinguish the notes, set a minimum time between them
    int pause_between_notes = note_duration * 0.8;
    delay(pause_between_notes);
    noTone(8);
  }
}

void Start_Song() {
  remaining = PLAY_COUNT;
}

void Stop_Song() {
  remaining = 0;
}