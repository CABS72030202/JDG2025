#define TEMPO         60000
#define QUARTER_NOTE  1

#define MSPB                60000 / TEMPO
#define HALF_NOTE           2.0 * QUARTER_NOTE
#define WHOLE_NOTE          2.0 * HALF_NOTE
#define EIGHTH_NOTE         0.5 * QUARTER_NOTE
#define SIXTEENTH_NOTE      0.5 * EIGHTH_NOTE

const float DOT(const float note) {return 1.5 * note;}