#include "events.h"

#include <stdio.h>


void log_keyboard_event(const keyboard_event *event, size_t event_count) {
    for (size_t i = 0; i < event_count; ++i) {
        const keyboard_event *e = &event[i];
        printf("Event %zu: key %u, state %s\r\n",
               i, e->key, e->state == KEY_PRESSED ? "pressed" : "released");
    }
}
