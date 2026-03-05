#include "midi_din.h"

#include "definitions.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "midi_tx_inv.pio.h"

/*
 * This simple PIO program implements a DIN MIDI transmission.
 * Should be used with inverting 5 V buffer to get the correct MIDI signal levels.
 */

typedef struct {
    PIO pio;
    uint sm;
    uint pin;
} midi_pio_tx_t;

static void midi_pio_tx_init(midi_pio_tx_t *m, PIO pio, uint sm, uint pin, float baud) {
    m->pio = pio;
    m->sm  = sm;
    m->pin = pin;

    uint offset = pio_add_program(pio, &midi_tx_inv_program);

    pio_sm_config c = midi_tx_inv_program_get_default_config(offset);

    // OUT to a single pin
    sm_config_set_out_pins(&c, pin, 1);
    sm_config_set_set_pins(&c, pin, 1);

    // Shift out LSB first; autopull every 10 bits (start + 8 data + stop)
    sm_config_set_out_shift(&c, true, true, 10);

    // One loop iteration = 1 instruction + delay[7] = 8 cycles per bit
    // clkdiv = sys_clk / (baud * 8)
    float div = (float)clock_get_hz(clk_sys) / (baud * 8.0f);
    sm_config_set_clkdiv(&c, div);

    // Init GPIO for PIO, drive low at idle
    pio_gpio_init(pio, pin);
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);
    pio_sm_set_pins(pio, sm, 0); // idle low

    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_enabled(pio, sm, true);
}

static void midi_pio_send_byte(midi_pio_tx_t *m, uint8_t b) {
    // Build 10-bit frame LSB-first: start=1, data=inverted, stop=0
    // Transmit order (LSB->MSB): start, d0..d7 (inverted), stop
    uint16_t data_inv = (uint16_t)(~b) & 0xFFu;
    uint16_t frame10  = (data_inv << 1) | 0x1u; // stop bit is implicitly 0 at bit9
    pio_sm_put_blocking(m->pio, m->sm, frame10);
}

midi_pio_tx_t midi;

void init_din_midi() {
    midi_pio_tx_init(&midi, pio0, 0, MIDI_TX_PIN, 31250.0f);
}

void write_midi_packet(uint8_t *msg, uint8_t size) {
    for (uint8_t i = 0; i < size; ++i) {
        midi_pio_send_byte(&midi, msg[i]);
    }
}
