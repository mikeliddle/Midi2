/** -- updated 2024-05-09 --
 * Project: Midi2
 * File:    Midi.c
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
#include "Midi.h"

midi_state *midi_current_state = NULL;

void midi_init(int channel) {
    // Clear out any previous state.
    if (midi_current_state) {
        free(midi_current_state);
    }

    // initialize the state variable
    midi_current_state = (midi_state *)malloc(sizeof(midi_state));

    // set the channel and default state.
    midi_current_state->current_channel = channel;
    midi_current_state->current_status = 0;
}

void midi_handle_note_off(midi_msg msg) {
    uint8_t channel = MIDI_DECODE_CHANNEL(msg.status);
    if (channel != midi_current_state->current_channel) {
        // ignore message if it's not for this channel.
        return;
    }

    uint8_t pitch = MIDI_DECODE_DATA(msg.data[0]);

    // turn off pitch
}

void midi_handle_note_on(midi_msg msg) {
    uint8_t channel = MIDI_DECODE_CHANNEL(msg.status);
    if (channel != midi_current_state->current_channel) {
        // ignore message if it's not for this channel.
        return;
    }

    uint8_t velocity = MIDI_DECODE_DATA(msg.data[1]);

    if (velocity == 0) {
        // note off
        midi_handle_note_off(msg);
        return;
    }

    uint8_t pitch = MIDI_DECODE_DATA(msg.data[0]);

    // turn on pitch with velocity
}

void midi_handle_polyphonic_aftertouch(midi_msg msg) {
}

void midi_handle_control_change(midi_msg msg) {
}

void midi_handle_program_change(midi_msg msg) {
}

void midi_handle_channel_aftertouch(midi_msg msg) {
}

void midi_handle_pitch_bend(midi_msg msg) {
}

void midi_handle_system(midi_msg msg) {
}

void midi_init_msg(midi_msg *msg, uint8_t *data) {
    // handle running status.
    if (midi_current_state->current_status) {
        if (data[0] & 0x80) {
            // new status
            msg->status = data[0];
            msg->data = data + 1;

            // update status if it's not a clock message
            if (!msg->status & MIDI_CLOCK_TYPE) {
                midi_current_state->current_status = msg->status;
            }
            // else don't interrupt running status.
        } else {
            // running status
            if (MIDI_CAN_USE_RUNNING_STATUS(midi_current_state->current_status)) {
                msg->status = midi_current_state->current_status;
                msg->data = data;
            } else {
                msg->status = data[0];
                msg->data = data + 1;
            }
        }
    } else {
        // new status
        msg->status = data[0];
        msg->data = data + 1;
        // update status if it's not a clock message
        if (!msg->status & MIDI_CLOCK_TYPE) {
            midi_current_state->current_status = msg->status;
        }
        // else don't interrupt running status.
    }
}

void midi_recieve_data(uint8_t *data) {
    midi_msg msg;
    midi_init_msg(&msg, data);

    switch (MIDI_DECODE_TYPE(msg.status)) {
    case MIDI_NOTE_OFF:
        midi_handle_note_off(msg);
        break;
    case MIDI_NOTE_ON:
        midi_handle_note_on(msg);
        break;
    case MIDI_POLYPHONIC_AFTERTOUCH:
        midi_handle_polyphonic_aftertouch(msg);
        break;
    case MIDI_CONTROL_CHANGE:
        midi_handle_control_change(msg);
        break;
    case MIDI_PROGRAM_CHANGE:
        midi_handle_program_change(msg);
        break;
    case MIDI_CHANNEL_AFTERTOUCH:
        midi_handle_channel_aftertouch(msg);
        break;
    case MIDI_PITCH_BEND:
        midi_handle_pitch_bend(msg);
        break;
    case MIDI_SYSTEM:
        midi_handle_system(msg);
        break;
    default:
        // ignore unknown message
        break;
    }
}

void midi_send(midi_msg *msg) {
}

void midi_init_voice_message(midi_msg *msg, uint8_t note, uint8_t velocity,
    uint8_t type) {
    // set message bytes
    uint8_t status = type | midi_current_state->current_channel;
    uint8_t data[3] = {status, note, velocity};
    // create message
    midi_init_msg(msg, data);
}

void midi_send_note_on(uint8_t note, uint8_t velocity) {
    midi_msg msg;
    midi_init_voice_message(&msg, note, velocity, MIDI_NOTE_ON);
    // send message
    midi_send(&msg);
}

void midi_send_note_off(uint8_t note, uint8_t velocity) {
    midi_msg msg;
    midi_init_voice_message(&msg, note, velocity, MIDI_NOTE_OFF);
    // send message
    midi_send(&msg);
}

void midi_send_polyphonic_aftertouch(uint8_t note, uint8_t pressure) {
    midi_msg msg;
    midi_init_voice_message(&msg, note, pressure, MIDI_POLYPHONIC_AFTERTOUCH);
    // send message
    midi_send(&msg);
}

void midi_send_control_change(uint8_t control, uint8_t value) {
    midi_msg msg;
    midi_init_voice_message(&msg, control, value, MIDI_CONTROL_CHANGE);
    // send message
    midi_send(&msg);
}

void midi_send_program_change(uint8_t program) {
    midi_msg msg;
    midi_init_voice_message(&msg, program, 0, MIDI_PROGRAM_CHANGE);
    // send message
    midi_send(&msg);
}

void midi_send_channel_aftertouch(uint8_t pressure) {
    midi_msg msg;
    midi_init_voice_message(&msg, pressure, 0, MIDI_CHANNEL_AFTERTOUCH);
    // send message
    midi_send(&msg);
}

void midi_send_pitch_bend(uint8_t lsb, uint8_t msb) {
    midi_msg msg;
    midi_init_voice_message(&msg, lsb, msb, MIDI_PITCH_BEND);
    // send message
    midi_send(&msg);
}

void midi_send_clock_tick() {
    uint8_t data = MIDI_CLOCK_TICK;
    midi_msg msg;
    midi_init_msg(&msg, &data);
    // send message
    midi_send(&msg);
}
