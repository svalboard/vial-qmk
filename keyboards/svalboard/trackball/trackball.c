#include "quantum.h"
#include "pointing_device.h"
#include "pointing_device_internal.h"

extern const pointing_device_driver_t *real_device_driver;

static uint16_t trackball_cached_cpi = 0;

report_mouse_t pointing_device_driver_get_report(report_mouse_t mouse_report) {
    int16_t swap;

    mouse_report = real_device_driver->get_report(mouse_report);

    swap = mouse_report.x;
    mouse_report.x = -mouse_report.y;
    mouse_report.y = -swap;

    return mouse_report;
}

uint16_t pointing_device_driver_get_cpi(void) {
    return trackball_cached_cpi;
}

void pointing_device_driver_set_cpi(uint16_t cpi) {
    if (cpi != trackball_cached_cpi) {
        real_device_driver->set_cpi(cpi);
        trackball_cached_cpi = cpi;
    }
}
