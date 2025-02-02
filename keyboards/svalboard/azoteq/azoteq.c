#include "quantum.h"
#include "pointing_device.h"
#include "pointing_device_internal.h"
#include "axis_scale.h"

extern const pointing_device_driver_t real_device_driver;

static axis_scale_t h = { 1, 7, 0 };
static axis_scale_t v = { 1, 7, 0 };

report_mouse_t pointing_device_driver_get_report(report_mouse_t mouse_report) {

    mouse_report = real_device_driver.get_report(mouse_report);

    mouse_report.h = add_to_axis(&h, mouse_report.h);
    mouse_report.v = add_to_axis(&v, mouse_report.v);

    return mouse_report;
}
