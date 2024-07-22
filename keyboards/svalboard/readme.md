# Svalboard RP2040

![Svalboard](https://www.svalboard.com)

Svalboard - Datahand Forever!

Keyboard Maintainer: [morganvenable](https://github.com/svalboard/vial-qmk) [@_claussen] on discord
Hardware Supported: RP2040, Svalboard Alpha, Svalboard Lightly
Hardware Availability: [Store](https://www.svalboard.com)

Build example for this keyboard (after setting up your build environment):
for Vial:
e.g.
```
make svalboard/right:vial
make svalboard/left:vial
```
or
```
make svalboard/trackpoint/right:vial
make svalboard/trackpoint/left:vial
```

--------------------------------------------------------------

For vanilla QMK (not maintained):

```
qmk compile -kb svalboard -km default
```
You can then copy the UF2 file from your qmk_firmware root directory to the device's drive letter.

Or you can flash directly with:
```
qmk flash --keyboard svalboard --keymap default
```

Svalboard currently uses the EE_HANDS flag to set handedness, so the MCU is either Left or Right, permanently. If for some reason you need to reflash this parameter to be the other hand, you must add the appropriate side bootloader parameter to program the eeprom -- this cannot be done by drag-and-drop of a UF2 file:
```
qmk flash --keyboard svalboard --keymap default -bl uf2-split-left
qmk flash --keyboard svalboard --keymap default -bl uf2-split-right
```

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader
To enter bootloader mode, simply double tap reset in 500ms or less.  The device will come up as a mass storage device titled RPI-RP2.  Drag the desired UF2 file to the drive and it will immediately reboot and enumerate as Svalboard.

## Svalboard-specific firmware features
See [Svalboard firmware features](docs/firmware_features.md) pages for information.
