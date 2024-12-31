#define TEMPO         80
#define QUARTER_NOTE  7

#define HALF_NOTE           2.0 * QUARTER_NOTE
#define WHOLE_NOTE          2.0 * HALF_NOTE
#define EIGHTH_NOTE         0.5 * QUARTER_NOTE
#define SIXTEENTH_NOTE      0.5 * EIGHTH_NOTE

const int DOT(const int note) {return 1.5 * note;}