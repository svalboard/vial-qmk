# Flashing Firmware

* [Flashing Firmware](#flashing-firmware)
  * [Before You Begin](#before-you-begin)
  * [Introduction](#introduction)
  * [Finding And Downloading The Right Firmware](#finding-and-downloading-the-right-firmware)
  * [Flashing Your Downloaded Firmware](#flashing-your-downloaded-firmware)
  * [Troubleshooting](#troubleshooting)

## Before You Begin

If you've done any customization of your layout, make sure you **save your
layout** in Vial! Flashing firmware will cause the loss of your current layout.

![The "Save current layout" entry in the "File" menu of Vial, along with the Ctrl+S shortcut.](images/save_vil.png)

## Introduction

While your Svalboard should work great with the firmware on it, if you're doing
a new build or want the latest features, you may want to update to the most
recent firmware release.

## Finding And Downloading The Right Firmware

To get the latest firmware, go to the
[releases](https://github.com/svalboard/vial-qmk/releases) page and find the
"Assets" of the latest release (it will be the first at the top of the page).

![The "Assets" of a release, showing the different files that can be downloaded,
e.g. svalboard_left_vial_v0.0.9sniper.uf2](images/firmware_assets.png)

You will need both a **left** and a **right** file, for the left and right
halves, respectively. The correct file is determined by which pointing device,
if any, you have installed on that specific side.

The files are named as `svalboard[_pointing_device]_<side>_vial_v<version>.uf2`,
where `[_pointing_device]` is the pointing device (if any), `<side>` is either
`left` or `right`, and `version` is the release number/name.

For instance, if you have no pointing device on the left and a trackpoint on the
right, download

* `svalboard_left_vial_v***.uf2` for the left
* `svalboard_trackpoint_right_vial_v***.uf2` for the right

(where `***` is whatever the current version is, e.g. `0.0.9sniper` in the
above image).

## Flashing Your Downloaded Firmware

Once you've located and downloaded both files you'll need, you will need to
flash each Svalboard half independently. To flash a half:

**Make certain you have saved your layout in Vial!**

* Ensure that that side (right or left) is plugged into your computer as the
  primary/master. The USB cable connected to your computer should be plugged
  into the port labeled `U`, **not** the one labeled `S`.
* Flip the half over and find the button on the underside labeled `RESET`.
* Press this button twice in rapid succession (in the way you would double-click
  something on a computer).
* In a few moments, your computer should detect a new removable drive called
  `RPI-RP2`.
* Navigate to that drive in the file explorer of your choice (as you would a
  thumb drive).
* Drag-and-drop or otherwise paste the correct firmware file you downloaded
  before into the detected drive.
* In a few moments, the Svalboard will reboot, the removable drive will
  disappear, and your new firmware will be flashed.

**Don't forget to do the other half, which will require changing which side the
USB cable is plugged into!**

Once you're finished, reload your saved Vial layout.

![The "Load saved layout" entry in the "File" menu of Vial, along with the
Ctrl+O shortcut.](images/load_vil.png)

## Troubleshooting

Once both halves are flashed, if things don't seem to work, try unplugging and
replugging the Svalboard. **Make certain you do not actuate any keys (especially
thumb) while the keyboard is booting.** It may sound like rote tech support
advice, but sometimes it just needs to reboot after a firmware update.

If it's still not working, make certain you flashed both sides with the correct
firmware (left for left, right for right) and that both sides are on the same
firmware release.

If your pointing device isn't working, make certain you downloaded the correct
firmware for that device. Note that there are **two different trackball chips**;
you most likely have the `pmw3389` version, but if you have an older trackball,
you may need the `pmw3360` version instead (it won't damage anything if you
flash the wrong one, your trackball just won't work until you flash the correct
one).

If you still can't figure it out, ask for help in [the
Discord](https://svalboard.com/discord) or find more options at
[https://svalboard.com/pages/support](https://svalboard.com/pages/support).
