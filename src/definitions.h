#ifndef MIDI_CONTROLLER_DEFINITIONS_H
#define MIDI_CONTROLLER_DEFINITIONS_H

// ===================== BUTTONS ======================

#define COLUMN_NUMBER   6
#define COLUMN_A_LENGTH 5
#define COLUMN_B_LENGTH 5
#define COLUMN_C_LENGTH 5
#define COLUMN_D_LENGTH 5
#define COLUMN_E_LENGTH 5
#define COLUMN_F_LENGTH 2

#define ROW_NUMBER      5
#define ROW_A_LENGTH    5
#define ROW_B_LENGTH    5
#define ROW_C_LENGTH    5
#define ROW_D_LENGTH    6
#define ROW_E_LENGTH    6

// Button Layout:
// Column:  A        B        C        D        E        F
// Row A:   []       []       []       []       []
// Row B:   []       []       []       []       []
// Row C:   []       []       []       []       []
// Row D:   []       []       []       []       []       []
// Row E:   []       []       []       []       []       []
// Rows    green  drive side (0 active)
// Columns blue   read side  (pull up)

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
