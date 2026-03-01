#include <stdio.h>

#include "definitions.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

void init_pins() {
    gpio_init(PIN_ROW_D);
    gpio_set_dir(PIN_ROW_D, GPIO_OUT);

    gpio_init(PIN_COLUMN_B);
    gpio_set_dir(PIN_COLUMN_B, GPIO_IN);
    gpio_pull_up(PIN_COLUMN_B);

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
}


inline bool read_pin() {
    return gpio_get(PIN_COLUMN_B);
}

inline void set_led(bool val) {
    gpio_put(PICO_DEFAULT_LED_PIN, val);
}

[[noreturn]] int main() {
    stdio_init_all();

    init_pins();

    printf("Initialized\r\n");

    for (;;) {
        gpio_put(PIN_ROW_D, false);

        const bool state = read_pin();

        // printf("Read %d\n", static_cast<int>(state));

        set_led(!state);

        sleep_ms(10);
    }
}