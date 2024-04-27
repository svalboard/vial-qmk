# MCU name
MCU = RP2040
BOOTLOADER = rp2040
BOARD = GENERIC_RP_RP2040

# we want some pretty lights
RGBLIGHT_ENABLE = yes
RGBLIGHT_DRIVER = ws2812
WS2812_DRIVER   = vendor

CUSTOM_MATRIX = lite

SRC += matrix.c $(EXTRA_SRC)

SERIAL_DRIVER = vendor
# this turns on trackpoint -- dupe in info.json?
# PS2_MOUSE_ENABLE = yes
# this turns on trackballs including pimoroni
POINTING_DEVICE_ENABLE = yes
# this selects trackball driver
POINTING_DEVICE_DRIVER = custom
# pimoroni_trackball
# pmw3389

REPEAT_KEY_ENABLE = yes

ifeq ($(strip $(POINTING_DEVICE_DRIVER)), pimoroni_trackball)
	OPT_DEFS += -DPOINTING_DEVICE_IS_PIMORONI
endif

ifeq ($(strip $(POINTING_DEVICE_DRIVER)), pmw3360)
	OPT_DEFS += -DPOINTING_DEVICE_IS_PMW3360
endif

ifeq ($(strip $(POINTING_DEVICE_DRIVER)), pmw3389)
	OPT_DEFS += -DPOINTING_DEVICE_IS_PMW3389
endif

# this turns on Manna-Harbour's automousekeys:
MH_AUTO_BUTTONS = yes

ifeq ($(strip $(MH_AUTO_BUTTONS)), yes)
  OPT_DEFS += -DMH_AUTO_BUTTONS
endif
