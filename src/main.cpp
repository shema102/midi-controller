#include <stdio.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "tusb.h"
#include "device/usbd.h"

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "kb/event/events.h"
#include "kb/handler/keyboard.h"
#include "midi/din/midi_din.h"


void init_pins() {
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
}

inline void set_led(bool val) {
    gpio_put(PICO_DEFAULT_LED_PIN, val);
}

static QueueHandle_t event_queue;

[[noreturn]] static void keyboard_poll_task(void *params) {
    (void) params;

    init_pins();
    init_keyboard_pins();

    keyboard_event event_buf[27];

    uint tick_count = 0;
    bool led_state = false;

    const TickType_t loop_period = pdMS_TO_TICKS(10);
    const TickType_t blink_period = pdMS_TO_TICKS(1000);
    TickType_t last_wake = xTaskGetTickCount();
    TickType_t last_blink = last_wake;

    for (;;) {
        size_t count = read_pin_state(event_buf, 27);

        // Send events to the queue and clear the buffer
        for (size_t i = 0; i < count; ++i) {
            xQueueSend(event_queue, &event_buf[i], 0);

            event_buf[i] = {};
        }

        TickType_t now = xTaskGetTickCount();
        if ((now - last_blink) >= blink_period) {
            last_blink += blink_period;
            led_state = !led_state;
            set_led(led_state);
            tick_count++;
            printf("Tick %u\r\n", tick_count);
        }

        vTaskDelayUntil(&last_wake, loop_period);
    }
}

[[noreturn]] static void process_key_events_task(void *params) {
    (void) params;

    init_din_midi();

    uint octave = 3;
    uint num_pressed = 0;

    keyboard_event ev{};
    for (;;) {
        if (xQueueReceive(event_queue, &ev, pdMS_TO_TICKS(1)) == pdTRUE) {
            log_keyboard_event(&ev, 1);

            if (ev.key == 25 && ev.state == KEY_PRESSED) {
                continue;
            }
            if (ev.key == 25 && ev.state == KEY_RELEASED) {
                if (octave > 1 && !num_pressed) {
                    octave--;
                    printf("Changed octave to %u\r\n", octave);
                }
                continue;
            }

            if (ev.key == 26 && ev.state == KEY_PRESSED) {
                continue;
            }
            if (ev.key == 26 && ev.state == KEY_RELEASED) {
                if (octave < 6 && !num_pressed) {
                    octave++;
                    printf("Changed octave to %u\r\n", octave);
                }
                continue;
            }

            uint8_t msg[3];
            msg[1] = ev.key + (octave * 12) + 24;

            if (ev.state == KEY_PRESSED) {
                msg[0] = 0x90; // note on
                msg[2] = 100;

                num_pressed++;
            } else {
                msg[0] = 0x80; // note off
                msg[2] = 0;
                num_pressed--;
            }

            tud_midi_n_stream_write(0, 0, msg, 3);

            write_midi_packet(msg, 3);
        }
    }
}

[[noreturn]] static void tusb_task(void *params) {
    (void) params;

    tusb_init();

    for (;;) {
        tud_task();
    }
}

[[noreturn]] int main() {
    stdio_init_all();

    event_queue = xQueueCreate(64, sizeof(keyboard_event));
    configASSERT(event_queue);

    TaskHandle_t keyboard_task_handle;
    TaskHandle_t event_task_handle;
    TaskHandle_t tusb_task_handle;

    BaseType_t ok;

    ok = xTaskCreate(keyboard_poll_task, "main_task", 2048, nullptr, 1, &keyboard_task_handle);
    configASSERT(ok == pdPASS);
    ok = xTaskCreate(process_key_events_task, "event_task", 2048, nullptr, 1, &event_task_handle);
    configASSERT(ok == pdPASS);
    ok = xTaskCreate(tusb_task, "tusb_task", 2048, nullptr, 1, &tusb_task_handle);
    configASSERT(ok == pdPASS);


    vTaskCoreAffinitySet(keyboard_task_handle, 1 << 0);
    vTaskCoreAffinitySet(event_task_handle, 1 << 0);

    vTaskStartScheduler();

    // Should never reach here
    for (;;) {
    }
}
