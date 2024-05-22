# this turns on trackballs including pimoroni
POINTING_DEVICE_ENABLE = yes
# this selects trackball driver
POINTING_DEVICE_DRIVER = pmw3389
# pimoroni_trackball
# pmw3389
MH_AUTO_BUTTONS = yes

ifeq ($(strip $(POINTING_DEVICE_DRIVER)), pimoroni_trackball)
	OPT_DEFS += -DPOINTING_DEVICE_IS_PIMORONI
endif

ifeq ($(strip $(POINTING_DEVICE_DRIVER)), pmw3360)
	OPT_DEFS += -DPOINTING_DEVICE_IS_PMW3360
endif

ifeq ($(strip $(POINTING_DEVICE_DRIVER)), pmw3389)
	OPT_DEFS += -DPOINTING_DEVICE_IS_PMW3389
endif

