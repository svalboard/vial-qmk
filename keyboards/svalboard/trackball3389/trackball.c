#include "quantum.h"
#include "pointing_device.h"
#include "pointing_device_internal.h"

extern const pointing_device_driver_t real_device_driver;

report_mouse_t pointing_device_driver_get_report(report_mouse_t mouse_report) {
    int16_t swap;

    mouse_report = real_device_driver.get_report(mouse_report);

    swap = mouse_report.x;
    mouse_report.x = -mouse_report.y;
    mouse_report.y = -swap;

    return mouse_report;
}
