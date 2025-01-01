#include "gandalf.h"

#define BUZZER_PIN 8
#define PLAY_COUNT 1   // The number of times the melody will be played (-1 is infinite)

void Play_Melody();
void Start_Song();
void Stop_Song();

int remaining;

void Play_Melody() {
  // Iterate over the notes of the melody
  for (int i = 0; i < NOTE_COUNT; i++) {

    // Calculate the note duration
    int note_duration = MSPB * note_durations[i];
    tone(BUZZER_PIN, note_pitches[i], note_duration);

    // To distinguish the notes, set a minimum time between them
    int pause_between_notes = note_duration * 0.8;
    delay(pause_between_notes);
    noTone(BUZZER_PIN);
  }
}

void Start_Song() {
  remaining = PLAY_COUNT;
  Play_Melody();
}

void Stop_Song() {
  remaining = 0;
}