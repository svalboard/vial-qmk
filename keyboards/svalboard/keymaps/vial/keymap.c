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

#include "../keymap_support.c"
#include "keycodes.h"
#include "quantum_keycodes.h"
#include QMK_KEYBOARD_H
#include <stdbool.h>
#include <stdint.h>
#include "svalboard.h"

layer_state_t default_layer_state_set_user(layer_state_t state) {
  sval_set_active_layer(0, false);
  return state;
}

layer_state_t layer_state_set_user(layer_state_t state) {
  sval_set_active_layer(get_highest_layer(state), false);
  return state;
}

enum layer {
    NORMAL,
    FUNC,
    NAS,
    BOARD_CONFIG,
    GAMEPAD,
    MBO = MH_AUTO_BUTTONS_LAYER,
};

#if __has_include("keymap_all.h")
#include "keymap_all.h"
#else
int sval_macro_size = 0;
uint8_t sval_macros[] = {0};
const uint16_t PROGMEM keymaps[DYNAMIC_KEYMAP_LAYER_COUNT][MATRIX_ROWS][MATRIX_COLS] = {
    [NORMAL] = LAYOUT(
        /*Center           North           East            South           West*/

        /*R1*/ KC_J,            KC_U,           KC_QUOTE,       KC_M,           KC_H, XXXXXXX,
        /*R2*/ KC_K,            KC_I,           KC_COLON,       KC_COMMA,       KC_Y, XXXXXXX,
        /*R3*/ KC_L,            KC_O,           KC_LGUI,        KC_DOT,         KC_N, XXXXXXX,
        /*R4*/ KC_SEMICOLON,    KC_P,           KC_BSLS,        KC_SLASH,       KC_RBRC, XXXXXXX,

        /*L1*/ KC_F,            KC_R,           KC_G,           KC_V,           KC_DOUBLE_QUOTE, XXXXXXX,
        /*L2*/ KC_D,            KC_E,           KC_T,           KC_C,           KC_GRAVE, XXXXXXX,
        /*L3*/ KC_S,            KC_W,           KC_B,           KC_X,           KC_ESC, XXXXXXX,
        /*L4*/ KC_A,            KC_Q,           KC_LBRC,        KC_Z,           KC_DEL, XXXXXXX,

        /*     Down            Pad            Up             Nail           Knuckle    DoubleDown*/
        /*RT*/ MO(NAS),         KC_SPACE,       TO(FUNC),       KC_BSPC,        KC_LALT,     XXXXXXX,
        /*LT*/ KC_LSFT,         KC_ENTER,       TO(NORMAL),          KC_TAB,         KC_LCTL,     KC_CAPS
        ),

    [FUNC] = LAYOUT(
        /*Center           North           East            South           West*/
        /*R1*/ KC_HOME,         KC_UP,          KC_RIGHT,       KC_DOWN,        KC_LEFT, XXXXXXX,
        /*R2*/ XXXXXXX,         KC_F8,          XXXXXXX,        KC_F7,          KC_END, XXXXXXX,
        /*R3*/ KC_PSCR,         KC_F10,         KC_LGUI,        KC_F9,          KC_INS, XXXXXXX,
        /*R4*/ KC_PAUSE,        KC_PGUP,        KC_F12,         KC_PGDN,        KC_F11, XXXXXXX,

        /*L1*/ KC_HOME,         KC_UP,          KC_RIGHT,       KC_DOWN,        KC_LEFT, XXXXXXX,
        /*L2*/ XXXXXXX,         KC_F6,          XXXXXXX,        KC_F5,          XXXXXXX, XXXXXXX,
        /*L3*/ TG(GAMEPAD),     KC_F4,          XXXXXXX,        KC_F3,          KC_ESC, XXXXXXX,
        /*L4*/ TO(BOARD_CONFIG), KC_F2,         XXXXXXX,        KC_F1,          KC_DEL, XXXXXXX,

        /*     Down            Pad            Up             Nail           Knuckle    DoubleDown*/
        /*RT*/ MO(NAS),         KC_SPACE,       _______,       KC_BSPC,      KC_LALT, _______,
        /*LT*/ KC_LSFT,         KC_ENTER,       _______,       KC_TAB,       KC_LCTL, _______
        ),

    [NAS] = LAYOUT(
        /*Center           North           East            South           West*/
        /*R1*/ KC_7,            KC_AMPR,        KC_UNDS,        KC_KP_PLUS,     KC_6, XXXXXXX,
        /*R2*/ KC_8,            KC_KP_ASTERISK, KC_COLON,       KC_COMMA,       KC_CIRCUMFLEX, XXXXXXX,
        /*R3*/ KC_9,            KC_LPRN,        KC_LGUI,        KC_DOT,         KC_SEMICOLON, XXXXXXX,
        /*R4*/ KC_0,            KC_RPRN,        XXXXXXX,        KC_QUES,        KC_RBRC, XXXXXXX,

        /*L1*/ KC_4,            KC_DOLLAR,      KC_5,           KC_MINUS,       KC_SLASH, XXXXXXX,
        /*L2*/ KC_3,            KC_HASH,        KC_GT,          KC_PERCENT,     KC_LT, XXXXXXX,
        /*L3*/ KC_2,            KC_AT,          XXXXXXX,        KC_X,           KC_ESC, XXXXXXX,
        /*L4*/ KC_1,            KC_EXCLAIM,     KC_TILDE,       KC_EQUAL,       KC_DEL, XXXXXXX,

        /*     Down            Pad            Up             Nail           Knuckle    DoubleDown*/
        /*RT*/ MO(NAS),         KC_SPACE,       _______,       KC_BSPC,        KC_LALT, _______,
        /*LT*/ KC_LSFT,         KC_ENTER,       _______,       KC_TAB,         KC_LCTL, _______
        ),

    [BOARD_CONFIG] = LAYOUT(
        /*                  Center              North               East                South               West                (XXX)               */
        /* R1 */            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            -1,
        /* R2 */            KC_TRNS,            RGB_VAI,            KC_TRNS,            RGB_VAD,            KC_TRNS,            -1,
        /* R3 */            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            -1,
        /* R4 */            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            -1,

        /* L1 */            SV_OUTPUT_STATUS,       KC_TRNS,             KC_TRNS,         KC_TRNS,            KC_TRNS,            -1,
        /* L2 */            SV_RIGHT_SCROLL_TOGGLE, SV_RIGHT_DPI_INC,    KC_TRNS,         SV_RIGHT_DPI_DEC,   KC_TRNS,            -1,
        /* L3 */            SV_LEFT_SCROLL_TOGGLE,  SV_LEFT_DPI_INC,     KC_TRNS,         SV_LEFT_DPI_DEC,    KC_TRNS,            -1,
        /* L4 */            SV_MH_CHANGE_TIMEOUTS,  SV_TOGGLE_ACHORDION, KC_TRNS,         KC_TRNS,            KC_TRNS,            -1,

        /*                  Down                Pad                 Up                  Nail                Knuckle             Double Down         */
        /* RT */            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,
        /* LT */            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS
      ),

    [GAMEPAD] = LAYOUT(
        /*                  Center              North               East                South               West                (XXX)               */
        /* R1 */            GC_SQU,             GC_TRI,             GC_CIR,             GC_CRO,             GC_SQU,             -1,
        /* R2 */            GC_TRI,             KC_NO,              KC_NO,              GC_CIR,             KC_NO,              -1,
        /* R3 */            GC_R1,              KC_NO,              KC_NO,              GC_R2,              KC_NO,              -1,
        /* R4 */            GC_L1,              KC_NO,              KC_NO,              GC_L2,              KC_NO,              -1,

        /* L1 */            GC_UNR,             GC_UNU,             GC_UNR,             GC_UND,             GC_UNL,             -1,
        /* L2 */            GC_UND,             GC_UNU,             KC_NO,              GC_UNU,             KC_NO,              -1,
        /* L3 */            GC_UNL,             KC_NO,              KC_NO,              KC_NO,              KC_NO,              -1,
        /* L4 */            GC_SEL,             GC_STA,             GC_TOG,             GC_HOM,             GC_SCD,             -1,

        /*                  Down                Pad                 Up                  Nail                Knuckle             Double Down         */
        /* RT */            GC_UNU,             GC_R3,              KC_NO,              KC_NO,              KC_NO,              KC_NO,
        /* LT */            GC_UNU,             GC_L3,              TG(GAMEPAD),        KC_NO,              KC_NO,              KC_TRNS
      ),

    [MBO] = LAYOUT(
        /*Center           North           East            South           West*/
        /*R1*/ KC_BTN1,        KC_TRNS,       KC_TRNS,       KC_TRNS,       KC_TRNS, XXXXXXX,
        /*R2*/ KC_BTN3,        KC_TRNS,       KC_TRNS,       KC_TRNS,       KC_TRNS, XXXXXXX,
        /*R3*/ KC_BTN2,        KC_TRNS,       KC_TRNS,       KC_TRNS,       KC_TRNS, XXXXXXX,
        /*R4*/ SV_RECALIBRATE_POINTER,        KC_TRNS,       KC_TRNS,       KC_TRNS,       KC_TRNS, XXXXXXX,
        /*L1*/ KC_BTN1,        KC_TRNS,       KC_TRNS,       KC_TRNS,        KC_TRNS, XXXXXXX,
        /*L2*/ KC_BTN3,        KC_TRNS,       KC_TRNS,       SV_SNIPER_2,        KC_TRNS, XXXXXXX,
        /*L3*/ KC_BTN2,        KC_TRNS,       KC_TRNS,       SV_SNIPER_3,        KC_TRNS, XXXXXXX,
        /*L4*/ SV_RECALIBRATE_POINTER, KC_TRNS, KC_TRNS,       SV_SNIPER_5,      KC_TRNS, XXXXXXX,
        /*     Down            Pad            Up             Nail           Knuckle    DoubleDown*/
        /*RT*/ KC_TRNS,        KC_BTN1,       KC_TRNS,       KC_BTN2,       KC_TRNS,   KC_TRNS,
        /*LT*/ KC_TRNS,        KC_BTN1,       KC_TRNS,       KC_BTN2,       KC_TRNS,   KC_TRNS
        )

};
#endif

bool achordion_chord(uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record,
                     uint16_t other_keycode, keyrecord_t* other_record) {
    if (tap_hold_record->event.key.row == 0 || tap_hold_record->event.key.row == 5 ||
        other_record->event.key.row    == 0 || other_record->event.key.row    == 5) {
        return true;
    }

    return achordion_opposite_hands(tap_hold_record, other_record);
}

void keyboard_post_init_user(void) {
  // Customise these values if you need to debug the matrix
  //debug_enable=true;
  //debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;

#if __has_include("keymap_all.h")
  if (fresh_install) {
    sval_init_defaults();
  }
#endif
}

// Joystick Config
joystick_config_t joystick_axes[JOYSTICK_AXIS_COUNT] = {
	JOYSTICK_AXIS_VIRTUAL,
	JOYSTICK_AXIS_VIRTUAL,
	JOYSTICK_AXIS_VIRTUAL,
	JOYSTICK_AXIS_VIRTUAL,
	JOYSTICK_AXIS_VIRTUAL,
	JOYSTICK_AXIS_VIRTUAL,
};
