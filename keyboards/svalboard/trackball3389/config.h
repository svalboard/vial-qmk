/*
Copyright 2023 Morgan Venable @_claussen

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#undef PS2_MOUSE_ENABLE
// in config.h:

#ifdef PS2_MOUSE_ENABLE
  //#define SERIAL_PIO_USE_PIO1
  #define PS2_PIO_USE_PIO1
  #define PS2_RESET_PIN GP25
  #define PS2_CLOCK_PIN GP24
  #define PS2_DATA_PIN GP23
#endif

#if defined(POINTING_DEVICE_ENABLE)

  // Pointing device stuff

  #if defined(POINTING_DEVICE_IS_PIMORONI)

    #define I2C_DRIVER I2CD1
    #define I2C1_SDA_PIN GP18
    #define I2C1_SCL_PIN GP19
    #define PIMORONI_TRACKBALL_SCALE 5

  #endif

  #if (defined(POINTING_DEVICE_IS_PMW3360) || defined(POINTING_DEVICE_IS_PMW3389))
    // SPI stuff
    #define USE_FFC_SPI

    #ifdef USE_FFC_SPI
        #define SPI_DRIVER SPID1
        // for FFC connector
        #define SPI_SCK_PIN GP26
        // Use TX# pin from SPI set.
        #define SPI_MOSI_PIN GP27
        // Use RX# pin from SPI set.
        #define SPI_MISO_PIN GP24
        // PMW33XX stuff
        // Use CS# pin from SPI set. Might not actually have to be the CS# pin, since
        // there's supposed to be support for multiple PMW33XX sensors, with different
        // CS pins.
        #define PMW33XX_CS_PIN GP25
        //#define PMW33XX_CS_PIN_RIGHT GP25
      #else
    //for Separate Pimoroni Connector
        #define SPI_DRIVER SPID0
        #define SPI_SCK_PIN GP18
        // Use TX# pin from SPI set.
        #define SPI_MOSI_PIN GP19
        // Use RX# pin from SPI set.
        #define SPI_MISO_PIN GP16
        // PMW33XX stuff
        // Use CS# pin from SPI set. Might not actually have to be the CS# pin, since
        // there's supposed to be support for multiple PMW33XX sensors, with different
        // CS pins.
        #define PMW33XX_CS_PIN GP17
    #endif
  #endif
#endif
