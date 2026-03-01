#ifndef MIDI_CONTROLLER_DEFINITIONS_H
#define MIDI_CONTROLLER_DEFINITIONS_H

// ===================== BUTTONS ======================

#define COLUMN_NUMBER   6
#define ROW_NUMBER      5

#define ROW_A_LENGTH    5
#define ROW_B_LENGTH    5
#define ROW_C_LENGTH    5
#define ROW_D_LENGTH    6
#define ROW_E_LENGTH    6

// Button logical layout:
// Column:    A          B          C          D          E          F
// Row A:   [ 8]       [10]       [ 6]       [ 3]       [ 1]
// Row B:   [ 4]       [26]       [25]       [ 2]       [ 0]
// Row C:   [12]       [ 9]       [ 5]       [11]       [ 7]
// Row D:   [13]       [24]       [18]       [20]       [22]       [15]
// Row E:   [14]       [23]       [17]       [19]       [21]       [16]
// Rows    green  drive side (0 active)
// Columns blue   read side  (pull up)

// Button ID physical layout:
//             1    3         6     8    10         13    15         18     20     22
//25  26    0    2    4    5    7    9     11   12     14    16   17     19    21     23   24

// GPIOs

#define PIN_COLUMN_A 10
#define PIN_COLUMN_B  9
#define PIN_COLUMN_C 13
#define PIN_COLUMN_D 11
#define PIN_COLUMN_E 8
#define PIN_COLUMN_F 12

#define PIN_ROW_A     2
#define PIN_ROW_B     4
#define PIN_ROW_C     3
#define PIN_ROW_D     5
#define PIN_ROW_E     6

#endif //MIDI_CONTROLLER_DEFINITIONS_H
