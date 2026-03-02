#ifndef MIDI_CONTROLLER_EVENTS_H
#define MIDI_CONTROLLER_EVENTS_H
#include "pico/types.h"

enum EventType {
    KEY_PRESSED,
    KEY_RELEASED
};

typedef struct {
    EventType state;
    uint key;
} keyboard_event;

void log_keyboard_event(const keyboard_event *event, size_t event_count);

#endif //MIDI_CONTROLLER_EVENTS_H