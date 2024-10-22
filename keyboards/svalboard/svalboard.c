#include "svalboard.h"
#include "eeconfig.h"
#include "version.h"
#include "split_common/transactions.h"

saved_values_t global_saved_values;
const int16_t mh_timer_choices[4] = { 300, 500, 800, -1 }; // -1 is infinite.
uint8_t sval_active_layer = 0;

void write_eeprom_kb(void) {
    eeconfig_update_kb_datablock(&global_saved_values);
}

void read_eeprom_kb(void) {
    bool modified = false;
    eeconfig_read_kb_datablock(&global_saved_values);
    if (global_saved_values.version < 1) {
        global_saved_values.version = 1;
        global_saved_values.right_dpi_index=2;
        global_saved_values.left_dpi_index=2;
        modified = true;
    }
    if (global_saved_values.version < 2) {
        global_saved_values.version = 2;
        global_saved_values.mh_timer_index = 1;
        modified = true;
    }
    if (global_saved_values.version < 3) {
        global_saved_values.version = 3;
#define HSV(c) (struct layer_hsv) { (c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF}
        // Colors from chatgpt.
        global_saved_values.layer_colors[0] = HSV(0x55FFFF); // Green
        global_saved_values.layer_colors[1] = HSV(0x15FFFF); // Orange
        global_saved_values.layer_colors[2] = HSV(0x95FFFF); // Azure
        global_saved_values.layer_colors[3] = HSV(0x0BB0FF); // Coral
        global_saved_values.layer_colors[4] = HSV(0x2BFFFF); // Yellow
        global_saved_values.layer_colors[5] = HSV(0x80FF80); // Teal
        global_saved_values.layer_colors[6] = HSV(0x00FFFF); // Red
        global_saved_values.layer_colors[7] = HSV(0xEAFFFF); // Pink
        global_saved_values.layer_colors[8] = HSV(0xBFFF80); // Purple
        global_saved_values.layer_colors[9] = HSV(0xAAFFFF); // Blue
        global_saved_values.layer_colors[10] = HSV(0x6AFFFF); // Spring Green
        global_saved_values.layer_colors[11] = HSV(0x80FFFF); // Turquoise
        global_saved_values.layer_colors[12] = HSV(0x24FFFF); // Gold
        global_saved_values.layer_colors[13] = HSV(0x1ED9D9); // Goldenrod
        global_saved_values.layer_colors[14] = HSV(0x80FFFF); // Cyan
        global_saved_values.layer_colors[15] = HSV(0xD5FFFF); // Magenta
        modified = true;
    }
    // As we add versions, just append here.
    if (modified) {
        write_eeprom_kb();
    }
    sval_active_layer = 0;
}

static const char YES[] = "yes";
static const char NO[] = "no";

const char *yes_or_no(int flag) {
    if (flag) {
	return YES;
    } else {
	return NO;
    }
}

const uint16_t dpi_choices[] = { 200, 400, 800, 1200, 1600, 2400 }; // If we need more, add them.
#define DPI_CHOICES_LENGTH (sizeof(dpi_choices)/sizeof(dpi_choices[0]))

void output_keyboard_info(void) {
    char output_buffer[256];

    sprintf(output_buffer, "%s:%s @ %s\n", QMK_KEYBOARD, QMK_KEYMAP, QMK_VERSION);
    send_string(output_buffer);
    sprintf(output_buffer, "Left Ptr: Scroll %s, cpi: %d, Right Ptr: Scroll %s, cpi: %d\n",
	    yes_or_no(global_saved_values.left_scroll), dpi_choices[global_saved_values.left_dpi_index],
	    yes_or_no(global_saved_values.right_scroll), dpi_choices[global_saved_values.right_dpi_index]);
    send_string(output_buffer);
    sprintf(output_buffer, "Achordion: %s, MH Keys Timer: %d\n",
	    yes_or_no(!global_saved_values.disable_achordion),
	    mh_timer_choices[global_saved_values.mh_timer_index]);
    send_string(output_buffer);
}

void increase_left_dpi(void) {
    if (global_saved_values.left_dpi_index + 1 < DPI_CHOICES_LENGTH) {
        global_saved_values.left_dpi_index++;
        set_left_dpi(global_saved_values.left_dpi_index);
        write_eeprom_kb();
    }
}

void decrease_left_dpi(void) {
    if (global_saved_values.left_dpi_index > 0) {
        global_saved_values.left_dpi_index--;
        set_left_dpi(global_saved_values.left_dpi_index);
        write_eeprom_kb();
    }
}

void increase_right_dpi(void) {
    if (global_saved_values.right_dpi_index + 1 < DPI_CHOICES_LENGTH) {
        global_saved_values.right_dpi_index++;
        set_right_dpi(global_saved_values.right_dpi_index);
        write_eeprom_kb();
    }
}

void decrease_right_dpi(void) {
    if (global_saved_values.right_dpi_index > 0) {
        global_saved_values.right_dpi_index--;
        set_right_dpi(global_saved_values.right_dpi_index);
        write_eeprom_kb();
    }
}
// TODO: Still need to add code to save values.
void set_left_dpi(uint8_t index) {
    uprintf("LDPI: %d %d\n", index, dpi_choices[index]);
    pointing_device_set_cpi_on_side(true, dpi_choices[index]);
}

void set_right_dpi(uint8_t index) {
    uprintf("RDPI: %d %d\n", index, dpi_choices[index]);
    pointing_device_set_cpi_on_side(false, dpi_choices[index]);
}

void set_dpi_from_eeprom(void) {
    read_eeprom_kb();
    set_left_dpi(global_saved_values.left_dpi_index);
    set_right_dpi(global_saved_values.right_dpi_index);
}

void kb_sync_listener(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
    // Just a ping-pong, no need to do anything.
}

void keyboard_post_init_kb(void) {
    set_dpi_from_eeprom();
    keyboard_post_init_user();
    transaction_register_rpc(KEYBOARD_SYNC_A, kb_sync_listener);
    if (is_keyboard_master()) {
        sval_set_active_layer(sval_active_layer, false);
    }
}

bool is_connected = false;

void housekeeping_task_kb(void) {
    if (is_keyboard_master()) {
        static uint32_t last_ping = 0;
        if (timer_elapsed(last_ping) > 500) {
            presence_rpc_t rpcout = {0};
            presence_rpc_t rpcin = {0};
            if (transaction_rpc_exec(KEYBOARD_SYNC_A, sizeof(presence_rpc_t), &rpcout, sizeof(presence_rpc_t), &rpcin)) {
                if (!is_connected) {
                    is_connected = true;
                    sval_on_reconnect();
                }
            } else {
                is_connected = false;
            }
            last_ping = timer_read32();
        }
    }
}

void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    struct layer_hsv *cols;
    uint8_t layer;
    if (data[0] != SVAL_VIA_PREFIX) return;
    switch (data[1]) {
        case sval_id_get_protocol_version:
            data[0] = 's';
            data[1] = 'v';
            data[2] = 'a';
            data[3] = 'l';
            data[4] = SVAL_PROTO_VERSION & 0xFF;
            data[5] = (SVAL_PROTO_VERSION >> 8) & 0xFF;
            data[6] = (SVAL_PROTO_VERSION >> 16) & 0xFF;
            data[7] = (SVAL_PROTO_VERSION >> 24) & 0xFF;
            break;
        case sval_id_get_layer_hsv:
            layer = data[2];
            if (layer > 15) layer = 15;
            cols = &global_saved_values.layer_colors[layer];
            data[0] = cols->hue;
            data[1] = cols->sat;
            data[2] = cols->val;
            break;
        case sval_id_set_layer_hsv:
            // Parameters start at data[2].
            layer = data[2];
            if (layer > 15) layer = 15;
            cols = &global_saved_values.layer_colors[layer];
            if (cols->hue != data[3] || cols->sat != data[4] || cols->val != data[5]) {
                cols->hue = data[3];
                cols->sat = data[4];
                cols->val = data[5];
                write_eeprom_kb();
            }
            sval_set_active_layer(sval_active_layer, false);
            break;
    }
}

void sval_on_reconnect(void) {
    // Reset colors, or it won't communicate the right color.
    rgblight_sethsv_noeeprom(0, 0, 0);
    sval_set_active_layer(sval_active_layer, true);
}

void sval_set_active_layer(uint32_t layer, bool save) {
    if (layer > 15) layer = 15;
    sval_active_layer = layer;
    struct layer_hsv cols = global_saved_values.layer_colors[layer];
    if (save) {
        rgblight_sethsv(cols.hue, cols.sat, cols.val);
    } else {
        rgblight_sethsv_noeeprom(cols.hue, cols.sat, cols.val);
    }
}

#ifndef SVALBOARD_REENABLE_BOOTMAGIC_LITE
// This is to override `bootmagic_lite` feature (see docs/feature_bootmagic.md),
// which can't be turned off in the usual way (via info.json) because setting
// `VIA_ENABLE` forces `BOOTMAGIC_ENABLE` in `builddefs/common_features.mk`.
//
// Obviously if you find this feature useful, you might want to define the
// SVALBOARD_... gating macro, and then possibly also (re-)define the
// `"bootmagic": { "matrix": [X,Y] },` in `info.json` to point the matrix at
// a more useful key than the [0,0] default. Ideally a center key, which is
// normally ~always present. Because the default (thumb knuckle) means that
// if you boot with the key pulled out, the eeprom gets cleared.

void bootmagic_lite(void) {
  // boo!
}
#endif

__attribute__((weak)) void recalibrate_pointer(void) {
}

