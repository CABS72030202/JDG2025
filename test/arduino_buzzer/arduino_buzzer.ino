#include "pitches.h"
#include "durations.h"

#define NOTE_COUNT 4

// notes in the melody:
int melody[NOTE_COUNT] = {
  NOTE_DS6, NOTE_E6, NOTE_CS6, NOTE_C6
};

float noteDurations[NOTE_COUNT] = {
  125, 500, 125, 1250
};

void setup() {
  Serial.begin(115200);
  Serial.println("-----");
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < NOTE_COUNT; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    int noteDuration = MSPB * noteDurations[thisNote];
    Serial.println(noteDuration);
    tone(8, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    int pauseBetweenNotes = noteDuration * 0.8;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
  }
}

void loop() {
  // no need to repeat the melody.
}
