# MCU name
MCU = RP2040
BOOTLOADER = rp2040
BOARD = GENERIC_RP_RP2040

# we want some pretty lights
RGBLIGHT_ENABLE = yes
RGBLIGHT_SPLIT = yes
RGBLIGHT_DRIVER = ws2812
WS2812_DRIVER   = vendor

CUSTOM_MATRIX = lite

SRC += matrix.c $(EXTRA_SRC)

SERIAL_DRIVER = vendor

POINTING_DEVICE_ENABLE = yes
POINTING_DEVICE_DRIVER = custom

REPEAT_KEY_ENABLE = yes

# this turns on Manna-Harbour's automousekeys:
MH_AUTO_BUTTONS = yes

ifeq ($(strip $(MH_AUTO_BUTTONS)), yes)
  OPT_DEFS += -DMH_AUTO_BUTTONS
endif
