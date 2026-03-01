#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "kb/events.h"
#include "kb/keyboard.h"

void init_pins() {
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
}

inline void set_led(bool val) {
    gpio_put(PICO_DEFAULT_LED_PIN, val);
}

keyboard_event event_buf[27];

[[noreturn]] int main() {
    stdio_init_all();

    init_pins();
    init_keyboard_pins();

    sleep_ms(1000);

    printf("Initialized\r\n");

    uint tick_count = 0;
    uint read_count = 0;

    for (;;) {

        size_t count = read_pin_state(event_buf, 27);

        log_keyboard_event(event_buf, count);

        // clear event buffer
        for (size_t i = 0; i < count; ++i) {
            event_buf[i] = {};
        }

        read_count++;

        if (read_count % 100 == 0) {
            tick_count++;
            set_led(tick_count % 2);

            printf("Tick %u\r\n", tick_count);
        }

        sleep_ms(10);
    }
}