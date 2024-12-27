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

#pragma once
#include <stdint.h>

// AXIS_TYPE must be signed.
#ifndef AXIS_TYPE
# define AXIS_TYPE int16_t
#endif

typedef struct {
    uint8_t   mult;
    uint8_t   div;
    AXIS_TYPE remainder;
} axis_scale_t;

AXIS_TYPE add_to_axis(axis_scale_t *, AXIS_TYPE);
void clear_remainder_axis(axis_scale_t *);

void mult_mult_axis(axis_scale_t *, uint8_t);
void div_mult_axis(axis_scale_t *, uint8_t);
void set_mult_axis(axis_scale_t *, uint8_t);

void mult_div_axis(axis_scale_t *, uint8_t);
void div_div_axis(axis_scale_t *, uint8_t);
void set_div_axis(axis_scale_t *, uint8_t);
