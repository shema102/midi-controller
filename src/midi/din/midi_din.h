#ifndef MIDI_CONTROLLER_MIDI_UART_H
#define MIDI_CONTROLLER_MIDI_UART_H
#include "pico/types.h"

void init_din_midi();

void write_midi_packet(uint8_t *msg, uint8_t size);

#endif //MIDI_CONTROLLER_MIDI_UART_H