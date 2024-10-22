/*
Copyright 2023 Morgan Venable @_claussen

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include "quantum.h"

extern const int16_t mh_timer_choices[4];

struct layer_hsv {
    uint8_t hue;
    uint8_t sat;
    uint8_t val;
};

struct saved_values {
    uint8_t version;  // Currently at 1,  We assume all new data will be zeroed.
    bool left_scroll :1;
    bool right_scroll :1;
    bool disable_achordion: 1;
    unsigned int unused0 :5;
    uint8_t left_dpi_index;
    uint8_t right_dpi_index;
    uint8_t mh_timer_index;
    struct layer_hsv layer_colors[DYNAMIC_KEYMAP_LAYER_COUNT];
};

#define SVAL_PROTO_VERSION 1

#define SVAL_VIA_PREFIX 0xEE

enum sval_command_ids {
    sval_id_get_protocol_version                 = 0x01,
    // Layer HSVs
    sval_id_get_layer_hsv                        = 0x10,
    sval_id_set_layer_hsv                        = 0x11,
};

// Just for ping pong, do we want anything else for it?
typedef struct _presence_rpc_t {
  int presence;
} presence_rpc_t;

typedef struct saved_values saved_values_t;

extern saved_values_t global_saved_values;
void output_keyboard_info(void);
void increase_left_dpi(void);
void increase_right_dpi(void);
void decrease_left_dpi(void);
void decrease_right_dpi(void);
void set_left_dpi(uint8_t index);
void set_right_dpi(uint8_t index);
void write_eeprom_kb(void);
void recalibrate_pointer(void);
void sval_set_active_layer(uint32_t layer, bool save);
void sval_on_reconnect(void);
