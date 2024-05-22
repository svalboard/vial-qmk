#pragma once

#include_next <halconf.h>

#if defined(POINTING_DEVICE_DRIVER)

#if defined(POINTING_DEVICE_IS_PIMORONI)

#undef HAL_USE_I2C
#define HAL_USE_I2C TRUE

#endif

#if (defined(POINTING_DEVICE_IS_PMW3360) || defined(POINTING_DEVICE_IS_PMW3389))

#undef HAL_USE_SPI
#define HAL_USE_SPI TRUE

#undef SPI_USE_WAIT
#define SPI_USE_WAIT TRUE

#undef SPI_SELECT_MODE
#define SPI_SELECT_MODE SPI_SELECT_MODE_PAD

#endif
#endif
