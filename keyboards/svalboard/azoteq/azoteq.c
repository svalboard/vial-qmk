#include "quantum.h"
#include "pointing_device.h"
#include "pointing_device_internal.h"
#include "axis_scale.h"
#include "svalboard.h"

// Azo can only report at 100hz.  Any faster is trouble.
#define AZO_MS 10

extern const pointing_device_driver_t *real_device_driver;

static uint16_t azo_timer = 0;
static uint8_t azo_held_buttons = 0;
static uint16_t azo_cached_cpi = 0;

report_mouse_t pointing_device_driver_get_report(report_mouse_t mouse_report) {
    if (timer_elapsed(azo_timer) >= AZO_MS) {
        azo_timer = timer_read();
        mouse_report        = real_device_driver->get_report(mouse_report);
        azo_held_buttons = mouse_report.buttons;
    }

    mouse_report.buttons = azo_held_buttons;

    return mouse_report;
}

uint16_t pointing_device_driver_get_cpi(void) {
    return azo_cached_cpi;
}

void pointing_device_driver_set_cpi(uint16_t cpi) {
    if (cpi != azo_cached_cpi) {
        real_device_driver->set_cpi(cpi);
        azo_cached_cpi = cpi;
    }
}
