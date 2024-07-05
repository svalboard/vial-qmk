/* SPDX-License-Identifier: GPL-2.0-or-later */
#pragma once

#define VIAL_KEYBOARD_UID {0x1B, 0x18, 0x7D, 0xF2, 0x21, 0xF6, 0x29, 0x48}

// Vial security combos, depending on which unit this is...
#ifdef INIT_EE_HANDS_RIGHT
// right thumb lock
#define VIAL_UNLOCK_COMBO_ROWS { 5, 5 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 1 }
#elif INIT_EE_HANDS_LEFT
// left thumb lock
#define VIAL_UNLOCK_COMBO_ROWS { 0, 0 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 1 }
#else
// both thumb locks
#define VIAL_UNLOCK_COMBO_ROWS { 0, 0, 5, 5 }
#define VIAL_UNLOCK_COMBO_COLS { 2, 5, 2, 5 }
#endif

// Shorten the unlock timeout (needs mod in `quantum/vial.c`; without
// it the override doesn't work)
#define VIAL_UNLOCK_COUNTER_MAX 12