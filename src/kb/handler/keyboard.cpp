#include "keyboard.h"

#include "../event/events.h"
#include "hardware/gpio.h"
#include "../../../config/definitions.h"
#include "hardware/timer.h"
#include "pico/time.h"

int row_pins[ROW_NUMBER] = {
    PIN_ROW_A,
    PIN_ROW_B,
    PIN_ROW_C,
    PIN_ROW_D,
    PIN_ROW_E
};

int column_pins[COLUMN_NUMBER] = {
    PIN_COLUMN_A,
    PIN_COLUMN_B,
    PIN_COLUMN_C,
    PIN_COLUMN_D,
    PIN_COLUMN_E,
    PIN_COLUMN_F
};

RowMask prev_kb_state[ROW_NUMBER] = {};
RowMask current_kb_state[ROW_NUMBER] = {};

void init_keyboard_pins() {
    for (const int row_pin: row_pins) {
        gpio_init(row_pin);
        gpio_set_dir(row_pin, GPIO_OUT);
        gpio_put(row_pin, true); // idle high; active low when selected
    }

    for (const int column_pin: column_pins) {
        gpio_init(column_pin);
        gpio_set_dir(column_pin, GPIO_IN);
        gpio_pull_up(column_pin);
    }
}

inline void set_kb_state(RowMask *state, Row row, Col col, bool val) {
    if (val) {
        state[row] |= (1 << col);
    } else {
        state[row] &= ~(1 << col);
    }
}

inline bool get_kb_state(const RowMask *state, Row row, Col col) {
    return (state[row] >> col) & 1u;
}

// count trailing zeros
static uint ctz(uint x) {
    // x must be non-zero
    return (uint) __builtin_ctz(x);
}

size_t read_pin_state(
    keyboard_event *event_buf,
    size_t event_buf_len
) {
    for (uint i = 0; i < ROW_NUMBER; i++) {
        prev_kb_state[i] = current_kb_state[i];
    }

    for (uint row = 0; row < ROW_NUMBER; row++) {
        // pull current row low
        gpio_put(row_pins[row], false);

        busy_wait_us(10);

        for (uint col = 0; col < rowCols[row]; col++) {
            bool is_pressed = !gpio_get(column_pins[col]);
            set_kb_state(current_kb_state, static_cast<Row>(row), static_cast<Col>(col), is_pressed);
        }

        // put it back
        gpio_put(row_pins[row], true);
    }

    // generate events
    size_t n = 0;

    for (uint row = 0; row < ROW_NUMBER; ++row) {
        RowMask valid = g_row_valid_mask[row];

        RowMask prev = prev_kb_state[row] & valid;
        RowMask curr = current_kb_state[row] & valid;

        RowMask changed = prev ^ curr;
        RowMask pressed = changed & curr; // 0->1
        RowMask released = changed & prev; // 1->0

        // Emit presses
        while (pressed && n < event_buf_len) {
            uint col = ctz(pressed);
            pressed = pressed & pressed - 1u; // clear lowest set bit
            event_buf[n++] = (keyboard_event){.state = KEY_PRESSED, .key = get_key_id(row, col)};
        }

        // Emit releases
        while (released && n < event_buf_len) {
            uint col = ctz(released);
            released = released & released - 1u;
            event_buf[n++] = (keyboard_event){.state = KEY_RELEASED, .key = get_key_id(row, col)};
        }

        if (n == event_buf_len) break; // buffer full
    }

    return n;
}
