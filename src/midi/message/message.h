#ifndef MIDI_CONTROLLER_MESSAGE_H
#define MIDI_CONTROLLER_MESSAGE_H
#include "pico/types.h"

constexpr uint8_t MIDI_BASE_NOTE = 24; // C in 2nd octave, which is the lowest note C on a standard 88-key piano
constexpr uint8_t SEMITONES_PER_OCTAVE = 12;

constexpr uint8_t MIDI_NOTE_ON = 0x90;
constexpr uint8_t MIDI_NOTE_OFF = 0x80;
constexpr uint8_t MIDI_CC = 0xB0;

typedef struct {
    uint8_t data[3];
} midi_message;

midi_message build_midi_message(uint8_t status, uint8_t data_1, uint8_t data_2);

midi_message get_note_on_message(uint8_t channel,uint8_t note, uint8_t velocity);
midi_message get_note_off_message(uint8_t channel, uint8_t note);

midi_message get_cc_message(uint8_t channel, uint8_t cc_number, uint8_t value);

uint8_t note_for_key(uint key_id, uint octave);

#endif //MIDI_CONTROLLER_MESSAGE_H