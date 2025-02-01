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

#include QMK_KEYBOARD_H
#include <stdbool.h>
#include <stdint.h>
#include "svalboard.h"

enum my_keycodes {
    SV_LEFT_DPI_INC = QK_KB_0,
    SV_LEFT_DPI_DEC,
    SV_RIGHT_DPI_INC,
    SV_RIGHT_DPI_DEC,
    SV_LEFT_SCROLL_TOGGLE,
    SV_RIGHT_SCROLL_TOGGLE,
    SV_RECALIBRATE_POINTER,
    SV_MH_CHANGE_TIMEOUTS,
    SV_CAPS_WORD,
    SV_TOGGLE_ACHORDION,
    SV_TOGGLE_23_67,
    SV_TOGGLE_45_67,
    SV_SNIPER_2,
    SV_SNIPER_3,
    SV_SNIPER_5,
    SV_SCROLL_HOLD,
    SV_SCROLL_TOGGLE,
    SV_OUTPUT_STATUS,
    GC_SQU,
    GC_CRO,
    GC_CIR,
    GC_TRI,
    GC_L1,
    GC_R1,
    GC_L2,
    GC_R2,
    GC_L3,
    GC_R3,
    GC_SEL,
    GC_STA,
    GC_HOM,
    GC_UND,
    GC_UNL,
    GC_UNR,
    GC_UNU,
    GC_DND,
    GC_DNL,
    GC_DNR,
    GC_DNU,
    GC_LND,
    GC_LNL,
    GC_LNR,
    GC_LNU,
    GC_RND,
    GC_RNL,
    GC_RNR,
    GC_RNU,
    GC_TOG,
    GC_TDP,
    GC_TLS,
    GC_TRS,
    GC_SCD,
    GC_UPLI,
    GC_LAST,
    GC_SUPN,
    GC_NEUT,
    // New keycodes should go here, to avoid breaking existing keymaps - order must match vial.json
    KC_NORMAL_HOLD = SAFE_RANGE,
    KC_FUNC_HOLD,
    SV_SAFE_RANGE, // Keycodes over this are safe on Svalboard.
};

#define MH_AUTO_BUTTONS_LAYER (DYNAMIC_KEYMAP_LAYER_COUNT - 1)
