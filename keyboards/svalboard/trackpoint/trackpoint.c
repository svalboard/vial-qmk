#include "quantum.h"
#include "pointing_device.h"
#include "pointing_device_internal.h"
#include "ps2_mouse.h"
#include "ps2.h"

extern const pointing_device_driver_t real_device_driver;

report_mouse_t pointing_device_driver_get_report(report_mouse_t mouse_report) {
    int16_t swap;

    mouse_report = real_device_driver.get_report(mouse_report);

    swap = mouse_report.x;
    mouse_report.x = mouse_report.y;
    mouse_report.y = -swap;

    return mouse_report;
}

void pointing_device_init_kb() {
//sensitivity command in fourth byte 00 = 0 multiplier, FF = 2.0 multiplier
  /*
    PS2_MOUSE_SEND(0xE2);
    PS2_MOUSE_SEND(0x81);
    PS2_MOUSE_SEND(0x4A);
    PS2_MOUSE_SEND(0x59);
    */
//Z tap -- doesn't seem to work.  0x01 is on.
  uint8_t val;

  ps2_mouse_disable_data_reporting();

  PS2_MOUSE_SEND(0xE2, "pts: 0xE2");
  PS2_MOUSE_SEND(0x47, "pts: 0x47");
  PS2_MOUSE_SEND(0x2C, "pts: 0x2C");
  PS2_MOUSE_SEND(0x00, "pts: 0x00");  // 01 turns on Z tap, 00 turns off
  PS2_MOUSE_RECEIVE("ztap - disable");

/*
From Sprintek:

E2 47 FA 10 - Disable drift correction. - Flips the bit, it should boot enabled.

This was given to us to stop the trackpoint from calibrating at bad times.

*/

  PS2_MOUSE_SEND(0xE2, "pts: 0xE2");
  PS2_MOUSE_SEND(0x80, "pts: 0x80");
  PS2_MOUSE_SEND(0xFA, "pts: 0xFA");
  val = ps2_host_recv_response();

  if (~val & 0x10) {
      PS2_MOUSE_SEND(0xE2, "pts: 0xE2");
      PS2_MOUSE_SEND(0x47, "pts: 0x47");
      PS2_MOUSE_SEND(0xFA, "pts: 0xFA");
      PS2_MOUSE_SEND(0x10, "pts: 0x10");
      PS2_MOUSE_RECEIVE("drift correction - disable");
  }
/*

From Sprintek:

E2 81 92 XX - Set the deadzone - 0x0C default, claimed.

Increase this to get rid of the tail drift.

*/
 PS2_MOUSE_SEND(0xE2, "pts: 0xE2");
 PS2_MOUSE_SEND(0x81, "pts: 0x81");
 PS2_MOUSE_SEND(0x92, "pts: 0x92");
 PS2_MOUSE_SEND(0x15, "pts: 0x15");  // Experiment with this if it isn't enough.

/*

Timing on the double tap on the trackpoint:

E2 80 5E
E2 81 5E XX

Z Time Constant [zTc]
 This PtS parameter is the time constant of the running average of Z force.  Its units are in
 force sampling time units as specified in the Set Sampling Rate command on page 14.
 This determines how sharp a press has to be to be recognized.  The value of this parameter
 is 256/n, where n is the desired time constant.  The running average is then calculated with
 the formula:
  where A = running average, N = present sample, and X is
 A=A+(N−A) X
 256
 zTc.  The default value for zTc is x"26" (decimal 38) corresponding to a time constant of
 6.7 samples.  This value is not affected by a reset (x"FF") or set default (x"F6") command

*/


    PS2_MOUSE_SEND(0xE2, "pts: 0xE2");
    PS2_MOUSE_SEND(0x81, "pts: 0x81");
    PS2_MOUSE_SEND(0x5E, "pts: 0x5E");
    PS2_MOUSE_SEND(0x00, "pts: 0x00");
    PS2_MOUSE_RECEIVE("double Z tap - disable");

    ps2_mouse_enable_data_reporting();

    pointing_device_init_user();

}


void recalibrate_pointer(void) {
    PS2_MOUSE_SEND(0xE2, "pts: 0xE2");
    PS2_MOUSE_SEND(0x51, "pts: 0x51");
    PS2_MOUSE_RECEIVE("trackpoint - recalibrate");
}
