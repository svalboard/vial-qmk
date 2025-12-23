#include "quantum.h"
#include "pointing_device.h"
#include "pointing_device_internal.h"
#include "axis_scale.h"

extern const pointing_device_driver_t *real_device_driver;


report_mouse_t pointing_device_driver_get_report(report_mouse_t mouse_report) {

    mouse_report = real_device_driver->get_report(mouse_report);
    mouse_report.y = -mouse_report.y;

    return mouse_report;
}
