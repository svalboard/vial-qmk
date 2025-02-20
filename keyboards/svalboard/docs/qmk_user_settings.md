# User settings
It is possible to set parameters via the QMK Settings tab in Vial. You will find
settings that control behavior of the keyboard that are usually not set dynamically.

## How to use
You must use a custom-built of the Svalboard. With that build, you will find the
QMK Settings tab. Under that tab, you should find a `Sval` tab. Change the values
as you would any other tab. Remember to save.

### Known bugs
Some behaviors can be modified with keys and through the user settings. If you have
the Sval settings tab open and use the keyboard keys to change a value, it will
not be refreshed in the Vial interface. Navigate away and back to see the new values.

The `Reset` button is only partially implemented so far.

It's not clear if the settings automouse works with the multiple reset paths.

## Adding new user setings

### Architecture
Vial and QMK communicate via the following protocol:
* Each setting has a an `uint16_t` id (`qsid`).
* Each setting has a byte width to allow for char, short, and long values.
* You are allowed to split a setting into subsettings with indivdual bits.
* We use a contiguous ID range.

Vial will query what `qsid`s are available to set before making queries. This
ensures that we can skip disabled values in future versions.

Then, we have setter and getters. They read and write from a buffer `setting` whose
allocated length is `sz`. For version compatibility and sanity checks we must
ensure that the width is the requested width.

We have four hooks set up in `qmk_settings.c`:
* `qmk_settings_query_user`: that is called by the Vial GUI to query what values can be configured.
* `qmk_settings_reset_user`: called to reset values. You can set RAM variables there. TODO(png): verify that this is used by the reset button and that we should also revert to default values.
* `qmk_settings_init_user`: called after reset, sets up any RAM values or code that need to be run after the setting has been reset.
* `qmk_settings_get_user`: used by the Vial GUI to read the keyboard value.
* `qmk_settings_set_notify_user`: set the value and calls any function that needs to be called after the value has changed, e. g. `set_left_dpi`.

The file contains only protocol code and shall not contain:
* init values
* keyboard logic
which should be relayed to `svalboard.c`.

### Adding a new value
To add a new value, go to `keyboards/svalboard/user_settings.c`. You will find `QsIdMap`.
Assign an identifier like `SVAL_QSID_MY_SETTING` and assign it the value `SVAL_QSID_NUM_VALUES`,
then increment the latter by `1`. Declare the byte width of the value as `SVAL_SZ_QSID_MY_SETTING`.

Handle the `get/set` switch. The underlying storage is in `keyboard/svalboard.h:gloval_saved_values`.

Once this is done, move to the vial GUI repo and modifiy `src/main/resources/base/qmk_settings.json`
with variable type and byte width, and qsid that you chose. Add a description.

### Modifying a value (backwards compatibility)
While the `QSID`s are not permanent, they are part of the Vial protocol. Bearing in mind that
people can go years without updating either the keyboard firmware and GUI, we'd strongly
advise not to override or modify existing behaviors.

Therefore, we advise just reitring the QSID value from the `query` interface, which should
prevent the value from displaying in the GUI. The JSON can keep the slot, allowing legacy firmware
to continue working.

## Issues with the approach
* Need matching vial GUI and firmware
* Forward path with dual dynamic (with keys) and static (with settings) is unclear
* Default (reset) values are in `svalboard.h` and should be exported
* The QSID is a dense array, so private branches will naturally have conflicting QSID values
  and be incompatible by design
