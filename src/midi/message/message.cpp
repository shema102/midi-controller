#include "message.h"

midi_message build_midi_message(const uint8_t status, const uint8_t data_1, const uint8_t data_2) {
    midi_message msg;
    msg.data[0] = status;
    msg.data[1] = data_1;
    msg.data[2] = data_2;
    return msg;
}

midi_message get_note_on_message(const uint8_t channel, const uint8_t note, const uint8_t velocity) {
    return build_midi_message(MIDI_NOTE_ON | (channel & 0x0F), note, velocity);
}

midi_message get_note_off_message(const uint8_t channel, const uint8_t note) {
    return build_midi_message(MIDI_NOTE_OFF | (channel & 0x0F), note, 0);
}

midi_message get_cc_message(uint8_t channel, uint8_t cc_number, uint8_t value) {
    return build_midi_message(MIDI_CC | (channel & 0x0F), cc_number, value);
}

// key_id is the id of the key in the button_key_map
uint8_t note_for_key(const uint key_id, const uint octave) {
    return MIDI_BASE_NOTE + key_id + octave * SEMITONES_PER_OCTAVE;
}

