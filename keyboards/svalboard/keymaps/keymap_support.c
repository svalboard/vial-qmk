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

#include QMK_KEYBOARD_H
#include <stdbool.h>
#include <stdint.h>
#include "svalboard.h"

#define MH_AUTO_BUTTONS_LAYER (DYNAMIC_KEYMAP_LAYER_COUNT - 1)
#define MH_AUTO_BUTTONS_TIMEOUT 5000
#define PS2_MOUSE_SCROLL_BTN_MASK (1<<PS2_MOUSE_BTN_MIDDLE) // this mask disables the key for non-PS2 purposes

// in keymap.c:
#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
void pointing_device_init_user(void) {
    set_auto_mouse_layer(MH_AUTO_BUTTONS_LAYER); // only required if AUTO_MOUSE_DEFAULT_LAYER is not set to index of <mouse_layer>
    set_auto_mouse_enable(true);         // always required before the auto mouse feature will work
}
#endif

enum my_keycodes {
    SV_LEFT_DPI_INC = QK_KB_0,
    SV_LEFT_DPI_DEC,
    SV_RIGHT_DPI_INC,
    SV_RIGHT_DPI_DEC,
    SV_LEFT_SCROLL_TOGGLE,
    SV_RIGHT_SCROLL_TOGGLE,
    SV_RECALIBRATE_POINTER,
    KC_NORMAL_HOLD = SAFE_RANGE,
    KC_FUNC_HOLD,
    SV_SAFE_RANGE, // Keycodes over this are safe on Svalboard.
};

#if (defined MH_AUTO_BUTTONS && defined PS2_MOUSE_ENABLE && defined MOUSEKEY_ENABLE)  || defined(POINTING_DEVICE_AUTO_MOUSE_MH_ENABLE)

static uint16_t mh_auto_buttons_timer;
extern int tp_buttons; // mousekey button state set in action.c and used in ps2_mouse.c

void mouse_mode(bool);

#endif

#if defined(POINTING_DEVICE_AUTO_MOUSE_MH_ENABLE)

#define SCROLL_DIVISOR 20

static int _ds_l_x = 0;
static int _ds_l_y = 0;
static int _ds_r_x = 0;
static int _ds_r_y = 0;

report_mouse_t pointing_device_task_combined_user(report_mouse_t reportMouse1, report_mouse_t reportMouse2) {
    if (reportMouse1.x == 0 && reportMouse1.y == 0 && reportMouse2.x == 0 && reportMouse2.y == 0)
        return pointing_device_combine_reports(reportMouse1, reportMouse2);

    if (global_saved_values.left_scroll) {
        int div_x;
        int div_y;

        _ds_l_x += reportMouse1.x;
        _ds_l_y -= reportMouse1.y;

        div_x = _ds_l_x / SCROLL_DIVISOR;
        div_y = _ds_l_y / SCROLL_DIVISOR;
        if (div_x != 0) {
            reportMouse1.h += div_x;
            _ds_l_x -= div_x * SCROLL_DIVISOR;
        }

        if  (div_y != 0) {
            reportMouse1.v += div_y;
            _ds_l_y -= div_y * SCROLL_DIVISOR;
        }
        reportMouse1.x = 0;
        reportMouse1.y = 0;
    }

    if (global_saved_values.right_scroll) {
        int div_x;
        int div_y;

        _ds_r_x += reportMouse2.x;
        _ds_r_y -= reportMouse2.y;

        div_x = _ds_r_x / SCROLL_DIVISOR;
        div_y = _ds_r_y / SCROLL_DIVISOR;
        if (div_x != 0) {
            reportMouse2.h += div_x;
            _ds_r_x -= div_x * SCROLL_DIVISOR;
        }

        if  (div_y != 0) {
            reportMouse2.v += div_y;
            _ds_r_y -= div_y * SCROLL_DIVISOR;
        }
        reportMouse2.x = 0;
        reportMouse2.y = 0;
    }

    if (mh_auto_buttons_timer) {
        mh_auto_buttons_timer = timer_read();
    } else {
        mouse_mode(true);
#if defined CONSOLE_ENABLE
        print("task - combined mh_auto_buttons: on\n");
#endif
    }
    return pointing_device_combine_reports(reportMouse1, reportMouse2);
}

report_mouse_t pointing_device_task_user(report_mouse_t reportMouse) {
    if (reportMouse.x == 0 && reportMouse.y == 0)
        return reportMouse;

    if (mh_auto_buttons_timer) {
        mh_auto_buttons_timer = timer_read();
    } else {
        mouse_mode(true);
#if defined CONSOLE_ENABLE
        print("user - mh_auto_buttons: on\n");
#endif
    }
    return reportMouse;
}
#endif


bool process_record_kb(uint16_t keycode, keyrecord_t *record) {

    // If console is enabled, it will print the matrix position and status of each key pressed
#ifdef CONSOLE_ENABLE
    uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif

#if (defined MH_AUTO_BUTTONS && defined PS2_MOUSE_ENABLE && defined MOUSEKEY_ENABLE) || defined(POINTING_DEVICE_AUTO_MOUSE_MH_ENABLE)
    if (mh_auto_buttons_timer) {
        switch (keycode) {
            case KC_BTN1:
            case KC_BTN2:
            case KC_BTN3:
            case KC_BTN4:
            case KC_BTN5:
            case KC_WH_U:
            case KC_WH_D:
            case KC_WH_R:
            case KC_WH_L:
            case KC_LSFT:
            case KC_RSFT:
            case KC_LCTL:
            case KC_RCTL:
            case KC_LALT:
            case KC_RALT:
            case KC_LGUI:
            case KC_RGUI:
            case SV_RECALIBRATE_POINTER:
                break;
            default:
#ifdef CONSOLE_ENABLE
                uprintf("process_record: off\n");
#endif
                mouse_mode(false);
        }
    }
    if (!record->event.pressed) {
        switch (keycode) {
            case SV_LEFT_DPI_INC:
                increase_left_dpi();
                break;
            case SV_LEFT_DPI_DEC:
                decrease_left_dpi();
                break;
            case SV_RIGHT_DPI_INC:
                increase_right_dpi();
                break;
            case SV_RIGHT_DPI_DEC:
                decrease_right_dpi();
                break;
            case SV_LEFT_SCROLL_TOGGLE:
                global_saved_values.left_scroll = !global_saved_values.left_scroll;
                write_eeprom_kb();
                break;
            case SV_RIGHT_SCROLL_TOGGLE:
                global_saved_values.right_scroll = !global_saved_values.right_scroll;
                write_eeprom_kb();
                break;
            case SV_RECALIBRATE_POINTER:
                recalibrate_pointer();
            default:
                break;
        }
    }
#endif

    return process_record_user(keycode, record);
};

#if defined MH_AUTO_BUTTONS && defined PS2_MOUSE_ENABLE && defined MOUSEKEY_ENABLE
void ps2_mouse_moved_user(report_mouse_t *mouse_report) {
    if (mh_auto_buttons_timer) {
        mh_auto_buttons_timer = timer_read();
    } else {
        if (!tp_buttons) {
            mouse_mode(true);
#if defined CONSOLE_ENABLE
            print("ps2 / mh_auto_buttons: on\n");
#endif
        }
    }
}
#endif



#if (defined MH_AUTO_BUTTONS && defined PS2_MOUSE_ENABLE && defined MOUSEKEY_ENABLE) || defined(POINTING_DEVICE_AUTO_MOUSE_MH_ENABLE)
void matrix_scan_kb(void) {
    if (mh_auto_buttons_timer && (timer_elapsed(mh_auto_buttons_timer) > MH_AUTO_BUTTONS_TIMEOUT)) {
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
    if (on) {
        layer_on(MH_AUTO_BUTTONS_LAYER);
        mh_auto_buttons_timer = timer_read();
    } else {
        layer_off(MH_AUTO_BUTTONS_LAYER);
        mh_auto_buttons_timer = 0;
    }
}

#endif // defined MH_AUTO_BUTTONS && defined PS2_MOUSE_ENABLE && #defined MOUSEKEY_ENABLE
