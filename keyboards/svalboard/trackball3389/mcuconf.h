#pragma once

#include_next "mcuconf.h"

#if defined(POINTING_DEVICE_IS_PIMORONI)
// TODO I2C0 does not work, for some reason.
#undef RP_I2C_USE_I2C1
#define RP_I2C_USE_I2C1 TRUE

#endif

#if (defined(POINTING_DEVICE_IS_PMW3360) || defined(POINTING_DEVICE_IS_PMW3389))
    #ifdef USE_FFC_SPI
        #undef RP_SPI_USE_SPI1
        #define RP_SPI_USE_SPI1 TRUE
    #else
        #undef RP_SPI_USE_SPI0
        #define RP_SPI_USE_SPI0 TRUE
    #endif
#endif
