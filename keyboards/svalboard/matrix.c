/*
Copyright 2012-2018 Jun Wako, Jack Humbert, Yiancar

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
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "util.h"
#include "matrix.h"
#include "debounce.h"
#include "quantum.h"
#include "print.h"

#define ROWS_PER_HAND 5

// matrix code
const pin_t col_pins[MATRIX_COLS] = MATRIX_COL_PINS;
const pin_t row_pins[ROWS_PER_HAND] = MATRIX_ROW_PINS;
//static const uint8_t col_pushed_states[MATRIX_COLS] = MATRIX_COL_PUSHED_STATES;
static const uint8_t col_pushed_states_fingers[MATRIX_COLS] = MATRIX_COL_PUSHED_STATES;
static const uint8_t col_pushed_states_thumbs[MATRIX_COLS] = MATRIX_COL_PUSHED_STATES_THUMBS;

static inline void setPinOutput_writeLow(pin_t pin) {
    ATOMIC_BLOCK_FORCEON {
        setPinOutput(pin);
        writePinLow(pin);
    }
}

static inline void setPinOutput_writeHigh(pin_t pin) {
    ATOMIC_BLOCK_FORCEON {
        setPinOutput(pin);
        writePinHigh(pin);
    }
}

static inline void setPinInputHigh_atomic(pin_t pin) {
    ATOMIC_BLOCK_FORCEON {
        setPinInputHigh(pin);
    }
}

static inline uint8_t readMatrixPin(pin_t pin) {
    if (pin != NO_PIN) {
        return (readPin(pin));
    } else {
        return 1;
    }
}


bool select_row(uint8_t row) {
    pin_t pin = row_pins[row];
    if (pin != NO_PIN) {
#ifdef PFET_ROWS
        setPinOutput_writeLow(pin);  //
        return true;
#else
        setPinOutput_writeHigh(pin);  //  this is opposite of most KB matrices
        return true;
#endif
    }
    return false;
}

void unselect_row(uint8_t row) {
    pin_t pin = row_pins[row];
#ifdef PFET_ROWS
            setPinOutput_writeHigh(pin);
#else
            setPinOutput_writeLow(pin);
#endif
}


static void unselect_rows(void) {
    for (uint8_t x = 0; x < ROWS_PER_HAND; x++) {
        unselect_row(x);
    }
}


/* matrix state(1:on, 0:off) */
extern matrix_row_t raw_matrix[ROWS_PER_HAND]; // raw values
extern matrix_row_t matrix[ROWS_PER_HAND];     // debounced values

int16_t scans_before_dd_detect = 3;  // Has to be one higher than the actual number of scans.
uint8_t dd_detected = 0;
void matrix_read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row) {
    // Start with a clear matrix row
    matrix_row_t current_row_value = 0;

    select_row(current_row);
    // if thumb row use col_pushed_states_thumbs

    wait_us(PREWAIT_US);

    // For each col...
    for (uint8_t col_index = 0; col_index < MATRIX_COLS; col_index++) {
        uint8_t pin_state;
        if (current_row == 0) {
            pin_state = (readPin(col_pins[col_index]) == col_pushed_states_thumbs[col_index]) ? 1 : 0;  // read pin and match pushed_states define
	    if (col_index == 5) { // DD
		if (scans_before_dd_detect >= 0) {
                   scans_before_dd_detect--;
		}
		if (!scans_before_dd_detect && scans_before_dd_detect == 0) {
		    dd_detected = pin_state;
		    scans_before_dd_detect--;
		}
		pin_state ^= dd_detected;
		pin_state &= 1;
	    }
        } else {
               pin_state = (readPin(col_pins[col_index]) == col_pushed_states_fingers[col_index]) ? 1 : 0; // read pin and match pushed_states define
        }
	// Populate the matrix row with the state of the col pin
        current_row_value |= (pin_state << col_index);
    }

    // Unselect row
    unselect_row(current_row);
    wait_us(POSTWAIT_US);

    // Update the matrix
    current_matrix[current_row] = current_row_value;
}

void matrix_init_custom(void) {
    print("matrix_init_custom\n");
    unselect_rows();
    for (uint8_t col = 0; col < MATRIX_COLS; col++) {
        pin_t pin = col_pins[col];
        if (pin != NO_PIN) {
            if (col == DOUBLEDOWN_COL){
                setPinInputHigh(pin);
            } else {
                setPinInput(pin);
            }
        }
    }
}
bool first_scan = true;
bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    
    matrix_row_t curr_matrix[ROWS_PER_HAND] = {0};
    // Set row, read cols
    for (uint8_t current_row = 0; current_row < (ROWS_PER_HAND); current_row++) {
        matrix_read_cols_on_row(curr_matrix, current_row);
    }

    // The first scan comes out backwards for reasons we don't understand.
    // Skipping it, stops the lockup experienced with thumbs out on
    // board boot.
    if (first_scan) {
        memset(raw_matrix, 0, sizeof(raw_matrix));
        first_scan = false;
	return true;
    } else {
        bool changed = memcmp(raw_matrix, curr_matrix, sizeof(curr_matrix)) != 0;
        if (changed) memcpy(raw_matrix, curr_matrix, sizeof(curr_matrix));
	return changed;
    }
}
