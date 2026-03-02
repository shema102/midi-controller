#include <stdio.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "kb/event/events.h"
#include "kb/handler/keyboard.h"
#include "pico/multicore.h"


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

    keyboard_event ev{};
    for (;;) {
        if (xQueueReceive(event_queue, &ev, pdMS_TO_TICKS(1)) == pdTRUE) {
            log_keyboard_event(&ev, 1);
        }
    }
}

[[noreturn]] int main() {
    stdio_init_all();

    event_queue = xQueueCreate(64, sizeof(keyboard_event));
    configASSERT(event_queue);

    TaskHandle_t keyboard_task_handle;
    TaskHandle_t event_task_handle;

    BaseType_t ok;

    ok = xTaskCreate(keyboard_poll_task, "main_task", 2048, nullptr, 1, &keyboard_task_handle);
    configASSERT(ok == pdPASS);
    ok = xTaskCreate(process_key_events_task, "event_task", 2048, nullptr, 1, &event_task_handle);
    configASSERT(ok == pdPASS);

    // affinity mask: run on core 1
    vTaskCoreAffinitySet(keyboard_task_handle, 1 << 0);
    vTaskCoreAffinitySet(event_task_handle, 1 << 1);

    // reset core 1 to ensure it starts running the scheduler
    // multicore_reset_core1();

    vTaskStartScheduler();

    // Should never reach here
    for (;;) {
    }
}
