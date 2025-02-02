/*

Copyright 2024 Ira Cooper <ira@wakeful.net>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/

#include "axis_scale.h"
#include <stdint.h>


void mult_mult_axis(axis_scale_t *as, uint8_t mul) {
    as->remainder *= mul;
    as->mult *= mul;
}

void div_mult_axis(axis_scale_t *as, uint8_t div) {
    if (div != 0) {
        uint8_t new_mult = as->mult / div;
        if (new_mult == 0) {
            new_mult = 1;
        }
        as->remainder /= div;
        as->mult = new_mult;
    }
}

void set_mult_axis(axis_scale_t *as, uint8_t val) {
    if (val != 0) {
        as->mult = val;
        as->remainder = 0;
    }
}

AXIS_TYPE add_to_axis(axis_scale_t *as, AXIS_TYPE val) {
    AXIS_TYPE ret_val;

    as->remainder += val * as->mult;

    ret_val = as->remainder / as->div;
    as->remainder -= ret_val * as->div;

    return ret_val;
}

void mult_div_axis(axis_scale_t *as, uint8_t mul) {
    as->remainder /= mul;
    if (as->remainder == 0 && mul != 0) {
        as->remainder = 1;
    }
    as->div *= mul;
}

void div_div_axis(axis_scale_t *as, uint8_t div) {
    if (div != 0) {
        uint8_t new_div = as->div / div;
        if (new_div == 0) {
            new_div = 1;
        }
        as->remainder *= div;
        as->div = new_div;
    }
}

void set_div_axis(axis_scale_t *as, uint8_t val) {
    if (val != 0) {
        as->div = val;
        as->remainder = 0;
    }
}

void clear_remainder_axis(axis_scale_t *as) {
    as->remainder = 0;
}
