#ifndef MIDI_CONTROLLER_KEYBOARD_H
#define MIDI_CONTROLLER_KEYBOARD_H
#include "../event/events.h"
#include "pico/types.h"
#include "../../../config/definitions.h"

enum Col {
    CA,
    CB,
    CC,
    CD,
    CE,
    CF
};

enum Row {
    RA,
    RB,
    RC,
    RD,
    RE
};

constexpr int rowCols[ROW_NUMBER] = {
    ROW_A_LENGTH,
    ROW_B_LENGTH,
    ROW_C_LENGTH,
    ROW_D_LENGTH,
    ROW_E_LENGTH
};

using RowMask = uint;
static constexpr RowMask g_row_valid_mask[ROW_NUMBER] = {
    ((1u << ROW_A_LENGTH) - 1u),
    ((1u << ROW_B_LENGTH) - 1u),
    ((1u << ROW_C_LENGTH) - 1u),
    ((1u << ROW_D_LENGTH) - 1u),
    ((1u << ROW_E_LENGTH) - 1u),
};

constexpr uint button_key_map[ROW_NUMBER][COLUMN_NUMBER] = {
    8, 10, 6, 3, 1, 0xFF,
    4, 26, 25, 2, 0, 0xFF,
    12, 9, 5, 11, 7, 0xFF,
    13, 24, 18, 20, 22, 15,
    14, 23, 17, 19, 21, 16,
};

constexpr uint get_key_id(uint row, uint col) {
    return button_key_map[row][col];
}

void init_keyboard_pins();

size_t read_pin_state(keyboard_event *event_buf,
                    size_t event_buf_len);

#endif //MIDI_CONTROLLER_KEYBOARD_H
