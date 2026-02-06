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
#include <complex.h>
#include <stdbool.h>
#include <stdint.h>
#include "svalboard.h"
#include "keymap_support.h"
#include "axis_scale.h"

// in keymap.c:
#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
void pointing_device_init_user(void) {
    set_auto_mouse_layer(MH_AUTO_BUTTONS_LAYER); // only required if AUTO_MOUSE_DEFAULT_LAYER is not set to index of <mouse_layer>
    set_auto_mouse_enable(true);         // always required before the auto mouse feature will work
}
#endif

#if (defined MH_AUTO_BUTTONS && defined PS2_MOUSE_ENABLE && defined MOUSEKEY_ENABLE) || defined(POINTING_DEVICE_AUTO_MOUSE_MH_ENABLE)

static uint16_t mh_auto_buttons_timer;
extern int tp_buttons; // mousekey button state set in action.c and used in ps2_mouse.c

void mouse_mode(bool);

#endif

#if defined(POINTING_DEVICE_AUTO_MOUSE_MH_ENABLE)


#define SCROLL_FREQUENCY_MS 10
#define SCROLL_DIVISOR 1
#define SCROLL_MULTIPLIER 1
bool mouse_mode_enabled = false;

axis_scale_t l_x = {1, SCROLL_DIVISOR, SCROLL_MULTIPLIER};
axis_scale_t l_y = {1, SCROLL_DIVISOR, SCROLL_MULTIPLIER};
axis_scale_t r_x = {1, SCROLL_DIVISOR, SCROLL_MULTIPLIER};
axis_scale_t r_y = {1, SCROLL_DIVISOR, SCROLL_MULTIPLIER};

#define MAC_DIVISOR 120
bool is_mac = false;
bool process_detected_host_os_kb(os_variant_t os) {
    if (!process_detected_host_os_user(os)) {
        return false;
    }

    switch (os) {
        case OS_MACOS:
        case OS_IOS:
            set_div_axis(&l_x, MAC_DIVISOR);
            set_div_axis(&l_y, MAC_DIVISOR);
            set_div_axis(&r_x, MAC_DIVISOR);
            set_div_axis(&r_y, MAC_DIVISOR);
	    is_mac = true;
            break;
        default:
            set_div_axis(&l_x, SCROLL_DIVISOR);
            set_div_axis(&l_y, SCROLL_DIVISOR);
            set_div_axis(&r_x, SCROLL_DIVISOR);
            set_div_axis(&r_y, SCROLL_DIVISOR);
	    is_mac = false;
	    break;
    }
    return true;
}

axis_scale_t sniper_x = {1, 1, 0};
axis_scale_t sniper_y = {1, 1, 0};
axis_scale_t sniper_h = {1, 1, 0};
axis_scale_t sniper_v = {1, 1, 0};

uint16_t scroll_timer = 0;
int16_t scroll_accumulator_h = 0;
int16_t scroll_accumulator_v = 0;
int32_t m_scroll_accumulator_h = 0;
int32_t m_scroll_accumulator_v = 0;

bool scroll_timer_running = false;

uint8_t sniper_hold_2 = 0;
uint8_t sniper_hold_3 = 0;
uint8_t sniper_hold_5 = 0;

bool sniper_toggle_2 = false;
bool sniper_toggle_3 = false;
bool sniper_toggle_5 = false;

#define any_sniper_active() (sniper_hold_2 + sniper_hold_3 + sniper_hold_5 + \
    sniper_toggle_2 + sniper_toggle_3 + sniper_toggle_5 > 0)

static bool scroll_hold    = false,
            scroll_toggle  = false;


#define AXIS_LOCK_BREAKAWAY_THRESHOLD 18750
#define AXIS_LOCK_ENGAGE_THRESHOLD 6250
#define AXIS_LOCK_TIMEOUT_MS 500

#define AXIS_DOMINANCE_NUMERATOR 3
#define AXIS_DOMINANCE_DENOMINATOR 2


static enum scroll_mode {
    SV_AXIS_FREE,
    SV_AXIS_LOCKED_V,
    SV_AXIS_LOCKED_H
} axis_scroll_mode = SV_AXIS_FREE;
static int32_t axis_lock_accum_h = 0;
static int32_t axis_lock_accum_v = 0;
static uint16_t axis_lock_timer = 0;


void update_axis_scroll_mode(int32_t h, int32_t v) {
    if (h != 0 || v != 0) {
        axis_lock_timer = timer_read();
    } else {
        if (timer_elapsed(axis_lock_timer) > AXIS_LOCK_TIMEOUT_MS) {
            if (axis_scroll_mode != SV_AXIS_FREE) {
                axis_scroll_mode = SV_AXIS_FREE;
                axis_lock_accum_h = 0;
                axis_lock_accum_v = 0;
		scroll_accumulator_h = 0;
                scroll_accumulator_v = 0;
		m_scroll_accumulator_h = 0;
		m_scroll_accumulator_v = 0;
            }
        }
        return;
    }

    switch (axis_scroll_mode) {
        case SV_AXIS_FREE:
            axis_lock_accum_h += h;
            axis_lock_accum_v += v;

            if (abs(axis_lock_accum_v) * AXIS_DOMINANCE_NUMERATOR >= abs(axis_lock_accum_h) * AXIS_DOMINANCE_DENOMINATOR) {
	        axis_scroll_mode = SV_AXIS_LOCKED_V;
                axis_lock_accum_h = 0;
            } else if (abs(axis_lock_accum_h) >= AXIS_LOCK_ENGAGE_THRESHOLD) {
                axis_scroll_mode = SV_AXIS_LOCKED_H;
                axis_lock_accum_v = 0;
            }
            break;

        case SV_AXIS_LOCKED_V:
            if ((int32_t)abs(v) * AXIS_DOMINANCE_NUMERATOR >= (int32_t)abs(h) * AXIS_DOMINANCE_DENOMINATOR) {
                axis_lock_accum_h = 0;
            }

	    axis_lock_accum_h += h;

            if (abs(axis_lock_accum_h) >= AXIS_LOCK_BREAKAWAY_THRESHOLD) {
	        axis_scroll_mode     = SV_AXIS_LOCKED_H;
                axis_lock_accum_v    = 0;
                scroll_accumulator_h = 0;
                scroll_accumulator_v = 0;
		m_scroll_accumulator_h = 0;
		m_scroll_accumulator_v = 0;
            }
        break;

        case SV_AXIS_LOCKED_H:
            if ((int32_t)abs(h) * AXIS_DOMINANCE_NUMERATOR >= (int32_t)abs(v) * AXIS_DOMINANCE_DENOMINATOR) {
                axis_lock_accum_v = 0;
            }

            axis_lock_accum_v += v;

            if (abs(axis_lock_accum_v) >= AXIS_LOCK_BREAKAWAY_THRESHOLD) {
                axis_scroll_mode     = SV_AXIS_LOCKED_V;
                axis_lock_accum_h    = 0;
                scroll_accumulator_h = 0;
                scroll_accumulator_v = 0;
		m_scroll_accumulator_h = 0;
		m_scroll_accumulator_v = 0;
            }
            break;
    }
}

void update_sniper_divisor(void) {
    uint8_t div = 1;
    for (uint8_t i = 0; i < sniper_hold_2 + sniper_toggle_2; i++) div *= 2;
    for (uint8_t i = 0; i < sniper_hold_3 + sniper_toggle_3; i++) div *= 3;
    for (uint8_t i = 0; i < sniper_hold_5 + sniper_toggle_5; i++) div *= 5;
    set_div_axis(&sniper_x, div);
    set_div_axis(&sniper_y, div);
    set_div_axis(&sniper_h, div);
    set_div_axis(&sniper_v, div);
}

report_mouse_t pointing_device_task_combined_user(report_mouse_t reportMouse1, report_mouse_t reportMouse2) {
    report_mouse_t ret_mouse;

    if (any_sniper_active()) {
        reportMouse1.x = add_to_axis(&sniper_x, reportMouse1.x);
        reportMouse1.y = add_to_axis(&sniper_y, reportMouse1.y);
        reportMouse1.h = add_to_axis(&sniper_h, reportMouse1.h);
        reportMouse1.v = add_to_axis(&sniper_v, reportMouse1.v);

        reportMouse2.x = add_to_axis(&sniper_x, reportMouse2.x);
        reportMouse2.y = add_to_axis(&sniper_y, reportMouse2.y);
        reportMouse2.h = add_to_axis(&sniper_h, reportMouse2.h);
        reportMouse2.v = add_to_axis(&sniper_v, reportMouse2.v);
    }

    if (reportMouse1.x == 0 && reportMouse1.y == 0 && reportMouse2.x == 0 && reportMouse2.y == 0)
        return pointing_device_combine_reports(reportMouse1, reportMouse2);

    if ((global_saved_values.left_scroll != scroll_hold) != scroll_toggle) {
        reportMouse1.h = add_to_axis(&l_x, reportMouse1.x);
        reportMouse1.v = add_to_axis(&l_y, -reportMouse1.y);

	
        reportMouse1.x = 0;
        reportMouse1.y = 0;
    }
    if ((global_saved_values.right_scroll != scroll_hold) != scroll_toggle) {
        reportMouse2.h = add_to_axis(&r_x, reportMouse2.x);
        reportMouse2.v = add_to_axis(&r_y, -reportMouse2.y);

        reportMouse2.x = 0;
        reportMouse2.y = 0;
    }

    if ((reportMouse1.h != 0 || reportMouse1.v != 0 || reportMouse2.h != 0 || reportMouse2.v != 0) && !scroll_timer_running) {
        scroll_timer_running = true;
        scroll_timer = timer_read();
    }

    if (scroll_timer_running) {
        m_scroll_accumulator_h += ((int32_t)reportMouse1.h * 100000) / get_left_dpi();
	m_scroll_accumulator_v += ((int32_t)reportMouse1.v * 100000) / get_left_dpi();
	m_scroll_accumulator_h += ((int32_t)reportMouse2.h * 100000) / get_right_dpi();
	m_scroll_accumulator_v += ((int32_t)reportMouse2.v * 100000) / get_right_dpi();

        scroll_accumulator_h += reportMouse1.h + reportMouse2.h;
        scroll_accumulator_v += reportMouse1.v + reportMouse2.v;
        reportMouse1.h = reportMouse2.h = 0;
        reportMouse1.v = reportMouse2.v = 0;
    }

    if (scroll_timer_running && timer_elapsed(scroll_timer) > SCROLL_FREQUENCY_MS) {
        if (global_saved_values.axis_scroll_lock && !is_mac) {
	    update_axis_scroll_mode(m_scroll_accumulator_h, m_scroll_accumulator_v);
            if (axis_scroll_mode == SV_AXIS_LOCKED_V) {
                reportMouse1.v = scroll_accumulator_v;
                reportMouse1.h = 0;
            } else {
                reportMouse1.h = scroll_accumulator_h;
                reportMouse1.v = 0;
            }
        } else {
            reportMouse1.h = scroll_accumulator_h;
            reportMouse1.v = scroll_accumulator_v;
        }

        scroll_timer_running = false;
        scroll_accumulator_h = 0;
        scroll_accumulator_v = 0;
	m_scroll_accumulator_h = 0;
	m_scroll_accumulator_v = 0;
    }

    mouse_mode(true);
    ret_mouse = pointing_device_combine_reports(reportMouse1, reportMouse2);

    return pointing_device_task_user(ret_mouse);
}

void toggle_axis_scroll_lock(void) {
    global_saved_values.axis_scroll_lock = !global_saved_values.axis_scroll_lock;
    write_eeprom_kb();
}

report_mouse_t pointing_device_task_user(report_mouse_t reportMouse) {
   if (reportMouse.x == 0 && reportMouse.y == 0 && reportMouse.h == 0 && reportMouse.v == 0)
        return reportMouse;

    mouse_mode(true);

    return reportMouse;
}
#endif

void mh_change_timeouts(void) {
    if (sizeof(mh_timer_choices) / sizeof(int16_t) - 1 <= global_saved_values.mh_timer_index) {
        global_saved_values.mh_timer_index = 0;
    } else {
        global_saved_values.mh_timer_index++;
    }
    uprintf("mh_timer:%d\n", mh_timer_choices[global_saved_values.mh_timer_index]);
    write_eeprom_kb();
}

#define LAYER_2345_MASK (0x3C)
void check_layer_67(void) {
    if ((layer_state & LAYER_2345_MASK) == LAYER_2345_MASK) {
        layer_on(6);
        layer_on(7);
    } else {
        layer_off(6);
        layer_off(7);
    }
}

bool in_mod_tap = false;
int8_t in_mod_tap_layer = -1;
int8_t mouse_keys_pressed = 0;
bool process_record_kb(uint16_t keycode, keyrecord_t *record) {

    // Abort additional processing if userspace code did
    if (!process_record_user(keycode, record)) { return false;}

    // We are in a mod tap, with a KC_TRANSPARENT, lets make it transparent...
    if (IS_QK_MOD_TAP(keycode) && ((keycode & 0xFF) == KC_TRANSPARENT) &&
        record->tap.count > 0 && !in_mod_tap &&
        in_mod_tap_layer == -1 && record->event.pressed) {

        in_mod_tap_layer = get_highest_layer(layer_state);
        layer_state = layer_state & ~(1 << in_mod_tap_layer);

        in_mod_tap = true;

        return true;
    }

    // Fix things up on the release for the mod_tap case.
    if (!record->event.pressed && in_mod_tap) {
        in_mod_tap = false;
        layer_state = layer_state | (1 << in_mod_tap_layer);
        in_mod_tap_layer = -1;
        return true;
    }

    // If console is enabled, it will print the matrix position and status of each key pressed
#ifdef CONSOLE_ENABLE
    uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif

    if (mouse_mode_enabled && layer_state & (1 << MH_AUTO_BUTTONS_LAYER)) {
        // The keycodes below are all that are forced to drop you out of mouse mode.
        // The intent is for this list to eventually become just KC_NO, and KC_TRNS
        // as more functionality is exported to keybard, and those keys are removed
        // from the firmmware. - ilc - 2024-10-05
#define BAD_KEYCODE_CONDITONAL  (keycode == KC_NO ||  \
	                    keycode == KC_TRNS || \
		            keycode == SV_LEFT_DPI_INC || \
	                    keycode == SV_LEFT_DPI_DEC || \
	                    keycode == SV_RIGHT_DPI_INC || \
	                    keycode == SV_RIGHT_DPI_DEC || \
	                    keycode == SV_LEFT_SCROLL_TOGGLE || \
		            keycode == SV_RIGHT_SCROLL_TOGGLE || \
		            keycode == SV_AXIS_SCROLL_LOCK || \
	                    keycode == SV_MH_CHANGE_TIMEOUTS || \
                        keycode == SV_TOGGLE_AUTOMOUSE)

        uint16_t layer_keycode = keymap_key_to_keycode(MH_AUTO_BUTTONS_LAYER, record->event.key);
        if (BAD_KEYCODE_CONDITONAL ||
	    layer_keycode != keycode) {
#ifdef CONSOLE_ENABLE
            uprintf("process_record - mh_auto_buttons: off\n");
#endif
            if (BAD_KEYCODE_CONDITONAL) {
                mouse_mode(false);
	        return false;
            } else {
                mouse_mode(false);
            }
        } else {
            if (record->event.pressed) {
                mouse_keys_pressed++;
                mouse_mode(true);
            } else {
                // keys that are held before the mouse layer is active can mess this up.
                if (mouse_keys_pressed > 0) {
                    mouse_keys_pressed--;
                }
                mouse_mode(true);
            }
        }
    }

    if (record->event.pressed) { // key pressed
        switch (keycode) {
            case SV_LEFT_DPI_INC:
                increase_left_dpi();
                return false;
            case SV_LEFT_DPI_DEC:
                decrease_left_dpi();
                return false;
            case SV_RIGHT_DPI_INC:
                increase_right_dpi();
                return false;
            case SV_RIGHT_DPI_DEC:
                decrease_right_dpi();
                return false;
            case SV_LEFT_SCROLL_TOGGLE:
                global_saved_values.left_scroll = !global_saved_values.left_scroll;
                write_eeprom_kb();
                return false;
            case SV_RIGHT_SCROLL_TOGGLE:
                global_saved_values.right_scroll = !global_saved_values.right_scroll;
                write_eeprom_kb();
                return false;
            case SV_RECALIBRATE_POINTER:
                recalibrate_pointer();
                return false;
            case SV_MH_CHANGE_TIMEOUTS:
                mh_change_timeouts();
                return false;
            case SV_CAPS_WORD:
                caps_word_toggle();
                return false;
            case SV_AXIS_SCROLL_LOCK:
                toggle_axis_scroll_lock();
                return false;
            case SV_TOGGLE_23_67:
                layer_on(2);
                layer_on(3);
                check_layer_67();
                return false;
            case SV_TOGGLE_45_67:
                layer_on(4);
                layer_on(5);
                check_layer_67();
                return false;
            case SV_SNIPER_2:
                sniper_hold_2++;
                update_sniper_divisor();
                return false;
            case SV_SNIPER_3:
                sniper_hold_3++;
                update_sniper_divisor();
                return false;
            case SV_SNIPER_5:
                sniper_hold_5++;
                update_sniper_divisor();
                return false;
            case SV_SNIPER_2_TG:
                sniper_toggle_2 = !sniper_toggle_2;
                update_sniper_divisor();
                return false;
            case SV_SNIPER_3_TG:
                sniper_toggle_3 = !sniper_toggle_3;
                update_sniper_divisor();
                return false;
            case SV_SNIPER_5_TG:
                sniper_toggle_5 = !sniper_toggle_5;
                update_sniper_divisor();
                return false;
            case SV_SCROLL_HOLD:
                scroll_hold = true;
                return false;
            case SV_SCROLL_TOGGLE:
                return false;
            case SV_OUTPUT_STATUS:
                output_keyboard_info();
                return false;
            case SV_TOGGLE_AUTOMOUSE:
                //if we disable automouse, manually kick out of mouse mode in case timer was running
                //needs to go first to avoid the lockout
                if (global_saved_values.auto_mouse) {
                    mouse_mode(false);
                }
                global_saved_values.auto_mouse = !global_saved_values.auto_mouse;
                write_eeprom_kb();
                return false;
	    case SV_TURBO_SCAN:
	        change_turbo_scan();
	        return false;
        }
    } else { // key released
        switch (keycode) {
            // These keys are all holds and require un-setting upon release.
            case SV_TOGGLE_23_67:
                layer_off(2);
                layer_off(3);
                check_layer_67();
                return false;
            case SV_TOGGLE_45_67:
                layer_off(4);
                layer_off(5);
                check_layer_67();
                return false;
            case SV_SNIPER_2:
                if (sniper_hold_2 > 0) sniper_hold_2--;
                update_sniper_divisor();
                return false;
            case SV_SNIPER_3:
                if (sniper_hold_3 > 0) sniper_hold_3--;
                update_sniper_divisor();
                return false;
            case SV_SNIPER_5:
                if (sniper_hold_5 > 0) sniper_hold_5--;
                update_sniper_divisor();
                return false;
            case SV_SNIPER_2_TG:
            case SV_SNIPER_3_TG:
            case SV_SNIPER_5_TG:
                return false;
            case SV_SCROLL_HOLD:
                scroll_hold = false;
                return false;
            case SV_SCROLL_TOGGLE:
                scroll_toggle ^= true;
                return false;
        }
    }

    // Neither the user nor the keyboard handled the event, so continue with normal handling
    return true;
};

#if defined MH_AUTO_BUTTONS && defined PS2_MOUSE_ENABLE && defined MOUSEKEY_ENABLE
void ps2_mouse_moved_user(report_mouse_t *mouse_report) {
    mouse_mode(true);
}
#endif

void matrix_scan_kb(void) {
    if ((mh_timer_choices[global_saved_values.mh_timer_index] >= 0) && mouse_mode_enabled && (timer_elapsed(mh_auto_buttons_timer) > mh_timer_choices[global_saved_values.mh_timer_index]) && mouse_keys_pressed == 0) {
        if (!tp_buttons) {
            mouse_mode(false);
#if defined CONSOLE_ENABLE
            print("matrix - mh_auto_buttons: off\n");
#endif
        }
    }

    matrix_scan_user();
}

void mouse_mode(bool on) {
    if (global_saved_values.auto_mouse) {
        if (on) {
            layer_on(MH_AUTO_BUTTONS_LAYER);
            mh_auto_buttons_timer = timer_read();
            mouse_mode_enabled = true;
        } else {
            layer_off(MH_AUTO_BUTTONS_LAYER);
            mh_auto_buttons_timer = 0;
            mouse_mode_enabled = false;
            mouse_keys_pressed = 0;
        }
    }
}
