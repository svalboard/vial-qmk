/* SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

#define VIAL_KEYBOARD_UID {0x1B, 0x18, 0x7D, 0xF2, 0x21, 0xF6, 0x29, 0x48}

// Vial unlock combos
#ifdef SVALBOARD_UNLOCK_BY_DEEP_PRESS
// both thumb locks
#define VIAL_UNLOCK_COMBO_ROWS { 0, 0, 5, 5 }
#define VIAL_UNLOCK_COMBO_COLS { 2, 5, 2, 5 }
#else
// both thumb down, without a lock
#define VIAL_UNLOCK_COMBO_ROWS { 0, 5 }
#define VIAL_UNLOCK_COMBO_COLS { 2, 2 }
#endif
