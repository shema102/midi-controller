#include "debug.h"

#include "definitions.h"
#include "hardware/gpio.h"

void init_debug() {
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    set_led(false);

    gpio_init(DEBUG_PIN);
    gpio_set_dir(DEBUG_PIN, GPIO_OUT);
    set_debug_pin(false);
}

void set_debug_pin(const bool value) {
    gpio_put(DEBUG_PIN, value);
}

void set_led(const bool value) {
    gpio_put(PICO_DEFAULT_LED_PIN, value);
}
