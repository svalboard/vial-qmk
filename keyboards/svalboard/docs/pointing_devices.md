# Pointing Devices

* [Pointing Devices](#pointing-devices)
  * [Introduction](#introduction)
    * [Current Options](#current-options)
    * [Deprecated Options](#deprecated-options)
  * [Physical Configuration](#physical-configuration)
  * [Automouse Layer](#automouse-layer)
    * [Leaving The Automouse Layer](#leaving-the-automouse-layer)
    * [By Button Press](#by-button-press)
      * [By Timeout](#by-timeout)
    * [Editing The Automouse Layer](#editing-the-automouse-layer)
    * [Changing The Timeout](#changing-the-timeout)
    * [Disabling The Automouse Layer](#disabling-the-automouse-layer)
  * [Adjusting Sensitivity](#adjusting-sensitivity)
    * [DPI](#dpi)
    * [Sniper Mode](#sniper-mode)
  * [Using A Pointing Device To Scroll](#using-a-pointing-device-to-scroll)
    * [Permanently](#permanently)
    * [Temporarily](#temporarily)
  * [Trackpoint Calibration/Drift](#trackpoint-calibrationdrift)
    * [Changing The Deadzone](#changing-the-deadzone)

## Introduction

The Svalboard supports up to one pointing device on each side.

Note that throughout this document, QMK keycodes are rendered as `KC_XYZ` (for a
standard keycode) or `SV_XYZ` (for a Svalboard-specific keycode). In
[Vial](https://get.vial.today)/[Keybard](https://captdeaf.github.io/keybard),
the corresponding keys have more human-readable names.

### Current Options

* Trackball (`pmw3389`)

  ![A trackball, mounted on the right half of a Svalboard.](images/trackball.jpg)

* Trackpad (`azoteq`)

  ![A trackpad, mounted on the right half of a Svalboard.](images/trackpad.jpg)

* Trackpoint

  ![A trackpoint, in isolation.](images/trackpoint.jpg)

### Deprecated Options

* Trackball (pmw3360) -- Note that the function of this older trackball module
  does not differ at all; it only requires selecting the correct firmware.

## Physical Configuration

There are no strict requirements as to which hand any pointing device is placed
on, either absolutely or relative to the connection to the computer. However,
for optimal function, it is **recommended that a trackpoint, if present, be
placed on the half connected to the computer (master)**. Due to firmware
limitations, a trackpoint on the non-host (slave) hand will feel "laggy" and
seem to move the cursor with a slow update speed.

## Automouse Layer

When you move a pointing device, the Svalboard will automatically activate a
layer intended for mouse keys. This layer (by default) provides mouse buttons on
the center keys, with index for left-click, ring for right-click, and middle for
middle-click. Additionally, trackpoint calibration is provided on pinky center.

The left-hand south keys have "sniper" keys, which may be held to temporarily
decrease the sensitivity of the pointer. Additionally, left- and right-click are
duplicated on the thumb pad/nail keys, respectively.

![A screenshot from Keybard showing the aforementioned layer](images/mouse_layer.png)

### Leaving The Automouse Layer

### By Button Press

Hitting any of the following keys (note that unmapped keys are included)
automatically leaves the automouse layer. By default, this means hitting most
any other key on the Svalboard that isn't mapped on the automouse layer, since
the remaining keys are "transparent".

The following keys will cause the automouse layer to deactivate:

* An unassigned " " or transparent "▽" key (`KC_NO` or `KC_TRNS`)
* Any key that permanently changes the state of the Svalboard, i.e.
  * Keys that change pointer DPI (`SV_LEFT_DPI_INC`, `SV_LEFT_DPI_DEC`,
    `SV_RIGHT_DPI_INC`, `SV_RIGHT_DPI_DEC`)
  * Keys that toggle a side's scroll state (`SV_LEFT_SCROLL_TOGGLE`,
    `SV_RIGHT_SCROLL_TOGGLE`)
  * Toggling achordion (`SV_TOGGLE_ACHORDION`)
  * Changing the automouse layer timer (`SV_MH_CHANGE_TIMEOUTS`)
  * Toggling the automouse layer (`SV_TOGGLE_AUTOMOUSE`)

#### By Timeout

There is also a timeout period for the automouse layer, that exits the layer if
the following are true:

* No pointing device has moved within the timer period.
* No key on the automouse layer has been pressed within the timer period.
* No key on the automouse layer is being held.

By default, this timer is 500ms. See
[Changing The Timeout](#changing-the-timeout) for more info.

### Editing The Automouse Layer

To edit the buttons on this layer, simply edit the last layer (layer 15) in
Vial/Keybard.

The following keys may be particularly useful on this layer:

* Mouse Buttons (`KC_BTN1`, `KC_BTN2`, `KC_BTN3`, `KC_BTN4`, `KC_BTN5`)
* Mouse Scrollwheel Up/Down/Right/Left (`KC_WH_U`, `KC_WH_D`, `KC_WH_R`,
  `KC_WH_L`)
* Modifiers (Shift, Control, Alt, etc.) (`KC_LSFT`, `KC_RSFT`, `KC_LCTL`,
  `KC_RCTL`, `KC_LALT`, `KC_RALT`, `KC_LGUI`, `KC_RGUI`)
* Scroll Toggle/Hold (`SV_SCROLL_TOGGLE`, `SV_SCROLL_HOLD`)
* "Sniper" Keys (`SV_SNIPER_2`, `SV_SNIPER_3`, `SV_SNIPER_5`)
* Trackpoint Calibration (`SV_RECALIBRATE_POINTER`)

### Changing The Timeout

The automouse layer timer may be cycled through four settings: 300ms, 500ms,
800ms, and infinite (in that order, wrapping around). To cycle to the next
timeout setting, bind the key `Mouse Key Timer` (`SV_MH_CHANGE_TIMEOUTS`)
somewhere in your layout and press it. *The Svalboard remembers this setting, so
you won't need to do it every time you plug in your keyboard.*

**Note that such permanent toggles cannot be placed on the automouse layer
itself, as keys that permanently change the Svalboards state are forbidden there
to avoid burning out the EEPROM.**

![The "Mouse Key Timer" key in the "User" tab of Vial.](images/vial_mouse_key_timer.png)

![The "Mouse Key Timer" key in the "Mouse and Special Keys" -> "Keyboard's
custom keys" tab of Keybard.](images/kb_mouse_key_timer.png)

### Disabling The Automouse Layer

The automouse layer may be toggled off/back on using the "AutoMouse Toggle"
(`SV_TOGGLE_AUTOMOUSE`) key. Binding it in your layout and tapping it once will
turn off the automatic switching (or back on, if it is already off). The
Svalboard remembers this toggle, so you won't need to do it every time you plug
in your keyboard.

**Note that such permanent toggles cannot be placed on the automouse layer
itself, as keys that permanently change the Svalboards state are forbidden there
to avoid burning out the EEPROM.**

![The "AutoMouse Toggle" key in the "User" tab of Vial.](images/vial_automouse_toggle.png)

![The "AutoMouse Toggle" key in the "Mouse and Special Keys" -> "Keyboard's
custom keys" tab of Keybard.](images/kb_automouse_toggle.png)

## Adjusting Sensitivity

### DPI

The DPI (sensitivity) of a left and right trackball may be adjusted
independently, using the left and right DPI +/- keys in the `User` tab of Vial
(`SV_LEFT_DPI_INC`, `SV_LEFT_DPI_DEC`, `SV_RIGHT_DPI_INC`, `SV_RIGHT_DPI_DEC`).
Bind these keys somewhere on your layout, and then press them to change the DPI
of the trackball.

**Note that these cannot be placed on the automouse layer, as keys that
permanently change the Svalboards state are forbidden there, to avoid burning
out the EEPROM.**

![The "Left/Right DPI +/-" keys in the "User" tab of Vial.](images/vial_dpi_keys.png)

![The "Left/Right DPI +/-" keys in the "Mouse and Special Keys" -> "Keyboard's
custom keys" tab of Keybard.](images/kb_dpi_keys.png)

Available DPIs are: 200, 400, 800, 1200, 1600, 2400. They do **not** wrap around
(i.e. pressing the button to increase DPI will never go from 2400 to 200).

*The Svalboard remembers this setting, so you won't need to do it every time you
plug in your keyboard.*

Please note that **the sensitivity of the trackpoint cannot be permanently
changed** and is unaffected by these keys.

### Sniper Mode

It is sometimes desirable to temporarily decrease the sensitivity of a pointing
device, if fine control is necessary in the moment.

To temporarily decrease the DPI (sensitivity) of all pointing devices, bind one
or more of the "sniper" keys in the `User` tab of Vial somewhere on your layout
(`SV_SNIPER_2`, `SV_SNIPER_3`, `SV_SNIPER_5`). While held down, these keys will
decrease the sensitivity of the pointing device by 2x, 3x, or 5x. Note that the
effects of these keys combine, e.g. holding both `Sniper 2x` and `Sniper 5x`
results in a 10-fold reduction in sensitivity. As soon as the keys are released,
sensitivity returns to normal.

![The "Sniper 2/3/5x" keys in the "User" tab of Vial.](images/vial_sniper_keys.png)

![The "Sniper 2/3/5x" keys in the "Mouse and Special Keys" -> "Keyboard's
custom keys" tab of Keybard.](images/kb_sniper_keys.png)

Note that **unlike DPI, sniper mode works with the trackpoint.**

## Using A Pointing Device To Scroll

One popular use of dual pointing devices is to use the left hand to scroll and
the right to move the cursor.

Each hand's pointing device can be independently set either to scroll or to move
the cursor.

### Permanently

To toggle a side between movement and scrolling, assign the "Scroll Left Toggle"
(`SV_LEFT_SCROLL_TOGGLE`) or "Scroll Right Toggle" (`SV_RIGHT_SCROLL_TOGGLE`)
keys in your layout.

Pressing the toggle for a side will switch that side's mode between scrolling
and cursor movement. The Svalboard remembers these left/right scroll toggles, so
you won't need to do it every time you plug in your keyboard. If you only want
to set a side up once, you can unbind the keys once you're done using them (or
leave them on a config layer or the like).

**Note that permanent toggles (`SV_LEFT_SCROLL_TOGGLE`,
`SV_RIGHT_SCROLL_TOGGLE`) cannot be placed on the automouse layer, as keys that
permanently change the Svalboards state are forbidden there to avoid burning out
the EEPROM.**

![The "Scroll Left/Right Toggle" keys in the "User" tab of Vial.](images/vial_permanent_scroll_toggle.png)

![The "Scroll Left/Right Toggle" keys in the "Mouse and Special Keys" -> "Keyboard's
custom keys" tab of Keybard.](images/kb_permanent_scroll_toggle.png)

### Temporarily

You might also wish to temporarily switch a side between scrolling and cursor
movement, for instance if you need to mouse one-handed. Two keys are provided
for this use:

* "Scroll Toggle" (`SV_SCROLL_TOGGLE`). Tap once to toggle both sides (scroll to
  cursor or cursor to scroll). This toggling does **not** persist between
  reboots; upon unplugging your Svalboard, it will default back to whatever
  state is permanently set ([see above](#permanently)).
* "Scroll Toggle Hold" (`SV_SCROLL_HOLD`). Hold down to toggle both sides
  (scroll to cursor or cursor to scroll). The devices return to their former
  setting once the key is released.

![The "Scroll Toggle/Hold" keys in the "User" tab of Vial.](images/vial_temp_scroll.png)

![The "Scroll Toggle/Hold" keys in the "Mouse and Special Keys" -> "Keyboard's
custom keys" tab of Keybard.](images/kb_temp_scroll.png)

## Trackpoint Calibration/Drift

In some circumstances, a trackpoint module may cause a tiny amount of cursor
drift. This is most often experienced with large temperature fluctuations. If
you notice your cursor drifting, you may press the "Fix Drift" key
(`SV_RECALIBRATE_POINTER`) to recalibrate the trackpoint's "zero" state. This
key is available on the default automouse layer as left/right pinky center, or
may be bound from the `User` tab in Vial.

![The "Fix Drift" key in the "User" tab of Vial.](images/vial_fix_drift.png)
![The "Fix Drift" key in the "Mouse and Special Keys" -> "Keyboard's
custom keys" tab of Keybard.](images/kb_fix_drift.png)

**Make certain you are not touching the trackpoint while you calibrate it.**

### Changing The Deadzone

If you find that drift occurs very often even with recalibration, you may need
to increase the deadzone of the trackpoint. This will require editing the code
in [trackpoint.c](../trackpoint/trackpoint.c) and recompiling the firmware.

The value on line 73:

```c
PS2_MOUSE_SEND(0x15, "pts: 0x15");  // Experiment with this if it isn't enough.
```

can be adjusted upwards slightly, for instance to `0x16` or `0x17` (it is
hexadecimal). **Make certain to adjust both instances of `0x15`.**

If you're not comfortable with this process, ask for help in
[the Discord](https://svalboard.com/discord) or find more options at
[https://svalboard.com/pages/support](https://svalboard.com/pages/support).
