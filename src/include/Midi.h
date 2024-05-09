/** -- updated 2024-05-09 --
 * Project: Midi2
 * File:    Midi.h
 * Author:  Mike Liddle
 * Created: 2024-05-09
 * License: MIT
 *
 * Copyright (c) 2024 Mike Liddle
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal 
 * in the Software without restriction, including without limitation the rights 
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions: 
 *  
 * The above copyright notice and this permission notice shall be included in all 
 * copies or substantial portions of the Software. 
 *  
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * Mike LiddleS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
 * SOFTWARE.
 **/
#ifndef _MIDI_H_
#define _MIDI_H_

#include <stdint.h>
#include "MidiDefs.h"

typedef struct midi_state_t {
    uint8_t current_status;
    uint8_t current_channel;
} midi_state;

typedef struct midi_msg_t {
    uint8_t status;
    uint8_t* data;
} midi_msg;

void midi_init(int channel);

void midi_init_msg(midi_msg* msg, uint8_t* data);

void midi_recieve_data(uint8_t* data);

void midi_handle_note_off(midi_msg msg);
void midi_handle_note_on(midi_msg msg);
void midi_handle_polyphonic_aftertouch(midi_msg msg);
void midi_handle_control_change(midi_msg msg);
void midi_handle_program_change(midi_msg msg);
void midi_handle_channel_aftertouch(midi_msg msg);
void midi_handle_pitch_bend(midi_msg msg);
void midi_handle_system(midi_msg msg);

void midi_send(midi_msg* msg);

void midi_send_note_on(uint8_t note, uint8_t velocity);
void midi_send_note_off(uint8_t note, uint8_t velocity);
void midi_send_polyphonic_aftertouch(uint8_t note, uint8_t pressure);
void midi_send_control_change(uint8_t control, uint8_t value);
void midi_send_program_change(uint8_t program);
void midi_send_channel_aftertouch(uint8_t pressure);
void midi_send_pitch_bend(uint8_t lsb, uint8_t msb);
void midi_send_clock_tick();

#endif  // _MIDI_H_
