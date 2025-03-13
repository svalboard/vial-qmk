# Compiling Custom (Userspace) Firmware/Keymaps For The Svalboard

* [Compiling Custom (Userspace) Firmware/Keymaps For The Svalboard](#compiling-custom-userspace-firmwarekeymaps-for-the-svalboard)
  * [Warning](#warning)
  * [Usecases](#usecases)
  * [Detecting The Svalboard](#detecting-the-svalboard)
  * [Accessing Svalboard-Specific Keycodes](#accessing-svalboard-specific-keycodes)
  * [Defining Your Own Custom Keycodes](#defining-your-own-custom-keycodes)
  * [The Svalboard Keymap](#the-svalboard-keymap)
    * [The Auto Mouse Button Layer](#the-auto-mouse-button-layer)
    * [Layer Count](#layer-count)
    * [Key Layout](#key-layout)
  * [Hard-coding Toggleable Values](#hard-coding-toggleable-values)
  * [LTO](#lto)

## Warning

**If you don't know what this is or why you'd want it, turn back!** Everything
you need can probably be done using Vial in a convenient, graphical program.

This document assumes you have a basic understanding of C and already know how
to compile userspace code for QMK. If you don't, you probably don't need this!

## Usecases

In some advanced cases, it may be desirable to compile custom
firmware/hard-coded keymaps for the Svalboard. For example:

* You need custom userspace code for advanced, non-standard features (adaptive
  keys, complex macros, keyloggers, etc.).
* You need to customize the behavior of features that *are* included (e.g.
  Achordion) to a degree not possible with Vial.
* You want to share configurations across multiple keyboards, some of which
  don't support Vial or it would be inconvenient to configure.
* You already have a QMK setup that "just works" and don't want to mess with
  Vial.

## Detecting The Svalboard

If you need to share code between the Svalboard and other keyboards, it can be
useful to detect whether or not the code is on the Svalboard, e.g. if you want
some behavior only on the Svalboard.

To this end, the Svalboard config defines the `SVALBOARD` macro. Thus, you can
write code that will run only on the Svalboard by checking if the macro is
defined. For example:

```c
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  ...
#ifdef SVALBOARD
  run_only_on_the_svalboard()
#else
  run_when_not_on_the_svalboard()
#endif
  ...
}
```

## Accessing Svalboard-Specific Keycodes

The Svalboard supports some keycodes that are unique to it, for instance,
`SV_RECALIBRATE_POINTER`, which recalibrates the deadzone of the trackpoint (if
installed). These are all prefixed with `SV_` as opposed to the usual `KC_`. To
use these in your `keymap.c`, include:

```c
#include "keymap_support.h"
```

in your `keymap.c`.

For a full list of available codes, please look in
[keymap_support.h](../keymaps/keymap_support.h) at the enum `my_keycodes`.

Note that you may need to add to your `rules.mk` to pull this in:

```makefile
VPATH += keyboards/svalboard/keymaps
SRC += keymap_support.c
```

## Defining Your Own Custom Keycodes

Since the Svalboard has its own custom keycodes, you must not use `SAFE_RANGE`
to define your own as you normally might, since these may then overlap with
Svalboard-specific keycodes. Instead, use `SV_SAFE_RANGE` defined in
`keymap_support.h` as a drop-in replacement (see
[Accessing Svalboard-Specific Keycodes](#accessing-svalboard-specific-keycodes)).

If you are sharing code across keyboards, you can do the following (see
[Detecting The Svalboard](#detecting-the-svalboard)):

```c
#ifdef SVALBOARD
#include "keymap_support.h"
#define RANGE_START SV_SAFE_RANGE
#else
#define RANGE_START SAFE_RANGE
#endif

enum custom_keycodes {
  CUSTOM_KEY_1 = RANGE_START,
  ANOTHER_CUSTOM_KEY,
  MORE_CUSTOM_KEYS,
};
```

## The Svalboard Keymap

### The Auto Mouse Button Layer

The index of the auto mouse button layer is defined in `keymap_support.h` and
can be accessed with `MH_AUTO_BUTTONS_LAYER`.

### Layer Count

The Svalboard defines `DYNAMIC_KEYMAP_LAYER_COUNT` for the total number of
layers; this can be used to define your keymap.

### Key Layout

Here is a brief example QWERTY keymap (not all layers shown). Note that fingers
are indexed from index out, i.e. R1 is "right index", R2 is "right middle, L4 is
"left pinky", etc.

Note that the keymap currently takes 6 keys per cluster, to support a second
south key. Your hardware most likely does not have those keys, so simply leave
them blank, as below.

Look in [keymaps/vial/keymap.c](../keymaps/vial/keymap.c) for the latest keymap
(as a starting place).

```c
const uint16_t PROGMEM keymaps[DYNAMIC_KEYMAP_LAYER_COUNT][MATRIX_ROWS][MATRIX_COLS] = {
    [PRIMARY] = LAYOUT(
            /* Center        North    East      South     West             Double-south */
        /*R1*/ KC_J,         KC_U,    KC_QUOTE, KC_M,     KC_H,            XXXXXXX,
        /*R2*/ KC_K,         KC_I,    KC_COLON, KC_COMMA, KC_Y,            XXXXXXX,
        /*R3*/ KC_L,         KC_O,    KC_LGUI,  KC_DOT,   KC_N,            XXXXXXX,
        /*R4*/ KC_SEMICOLON, KC_P,    KC_BSLS,  KC_SLASH, KC_RBRC,         XXXXXXX,

        /*L1*/ KC_F,         KC_R,    KC_G,     KC_V,     KC_DOUBLE_QUOTE, XXXXXXX,
        /*L2*/ KC_D,         KC_E,    KC_T,     KC_C,     KC_GRAVE,        XXXXXXX,
        /*L3*/ KC_S,         KC_W,    KC_B,     KC_X,     KC_ESC,          XXXXXXX,
        /*L4*/ KC_A,         KC_Q,    KC_LBRC,  KC_Z,     KC_DEL,          XXXXXXX,

            /* Down     Inner (pad)  Upper (mode)  Outer Upper (nail)  Outer Lower (knuckle)  Pushthrough */
        /*RT*/ MO(NAS), KC_SPACE,    TO(FUNC),     KC_BSPC,            KC_LALT,               TG(NAS),
        /*LT*/ KC_LSFT, KC_ENTER,    TO(NORMAL),   KC_TAB,             KC_LCTL,               KC_CAPS
    ),

    [MH_AUTO_BUTTONS_LAYER] = LAYOUT(
            /* Center                  North    East     South    West     Double-south */
        /*R1*/ KC_TRNS,                KC_TRNS, KC_TRNS, KC_BTN1, KC_TRNS, XXXXXXX,
        /*R2*/ KC_TRNS,                KC_TRNS, KC_TRNS, KC_BTN3, KC_TRNS, XXXXXXX,
        /*R3*/ KC_TRNS,                KC_TRNS, KC_TRNS, KC_BTN2, KC_TRNS, XXXXXXX,
        /*R4*/ SV_RECALIBRATE_POINTER, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, XXXXXXX,

        /*L1*/ KC_TRNS,                KC_TRNS, KC_TRNS, KC_BTN1, KC_TRNS, XXXXXXX,
        /*L2*/ KC_TRNS,                KC_TRNS, KC_TRNS, KC_BTN3, KC_TRNS, XXXXXXX,
        /*L3*/ KC_TRNS,                KC_TRNS, KC_TRNS, KC_BTN2, KC_TRNS, XXXXXXX,
        /*L4*/ SV_RECALIBRATE_POINTER, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, XXXXXXX,

            /* Down     Inner (pad)  Upper (mode)  Outer Upper (nail)  Outer Lower (knuckle)  Pushthrough */
        /*RT*/ KC_TRNS, KC_TRNS,     KC_TRNS,      KC_TRNS,            KC_TRNS,               KC_TRNS,
        /*LT*/ KC_TRNS, KC_TRNS,     KC_TRNS,      KC_TRNS,            KC_TRNS,               KC_TRNS
        )
};
```

## Hard-coding Toggleable Values

If you want to forcibly set toggleable values (e.g. the mouse keys timer), you
may do so in `keyboard_post_init_user()`. For example:

```c
void keyboard_post_init_user(void) {
  global_saved_values.mh_timer_index = 1; // Set mousekeys timer to 500 ms.
}
```

Changes made this way are *not* written to the EEPROM, but will be set at each
startup.

You will need to dig through [svalboard.c](../svalboard.c) and
[keymaps/keymap_support.c](../keymaps/keymap_support.c) to find the details of
what can be set this way.

## LTO

Do not enable Link Time Optimization. The Svalboard is not short on space, so
there is no need for it, and compiling will fail with it enabled.

```makefile
# DO NOT HAVE EITHER OF THESE UNCOMMENTED IN YOUR rules.mk
# LTO_ENABLE = yes
# EXTRAFLAGS += -flto
```
