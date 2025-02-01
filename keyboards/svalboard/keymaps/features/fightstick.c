#include "fightstick.h"
#include "svalboard.h"
#include QMK_KEYBOARD_H
#include "../keymap_support.h"
#include <stdbool.h>
#include <stdint.h>

//left, down, up, right
bool DP_STATE[] = {false, false, false, false};
bool RS_STATE[] = {false, false, false, false};
bool LS_STATE[] = {false, false, false, false};

void clear_button_axis(void) {
    joystick_set_axis(0, 0);
    joystick_set_axis(1, 0);
    joystick_set_axis(2, 0);
    joystick_set_axis(3, 0);
    unregister_joystick_button(15);
    unregister_joystick_button(16);
    unregister_joystick_button(17);
    unregister_joystick_button(18);
    for (int i = 0; i < 4; i++) {
        DP_STATE[i] = false;
        RS_STATE[i] = false;
        LS_STATE[i] = false;
    }
}

bool* handle_socd(bool pressed, bool *state, int direction) {
    state[direction] = pressed;
    bool *TEMP_STATE = malloc(4 * sizeof(bool));
    memcpy(TEMP_STATE, state, 4 * sizeof(bool));
    int opposite = 3 - direction;
    if (TEMP_STATE[direction] && TEMP_STATE[opposite]) {
        switch (global_saved_values.socd_mode) {
            case 0:
                //last input wins, up priority
                if (direction==0 || direction==3) {
                    TEMP_STATE[opposite] = false;
                }
                else {
                    TEMP_STATE[1] = false;
                }
                return TEMP_STATE;
            case 1:
                //last input wins
                TEMP_STATE[opposite] = false;
                return TEMP_STATE;
            case 2:
                //neutral, up priority
                if (direction==0 || direction==3) {
                    TEMP_STATE[opposite] = false;
                    TEMP_STATE[direction] = false;
                }
                else {
                    TEMP_STATE[1] = false;
                }
                return TEMP_STATE;
            case 3:
                //true neutral
                TEMP_STATE[opposite] = false;
                TEMP_STATE[direction] = false;
                return TEMP_STATE;
        }
    }
    return TEMP_STATE;
}

void handle_stick(int stick, bool *state) {
    //left stick axis is 0 for lr and 1 for ud, right stick is 3 and 4
    //stick 0 is for left stick, and stick 1 is for right stick
    int left_right = 0 + stick * 3;
    int up_down = left_right + 1;
    int lr_state = 0;
    int ud_state = 0;
    if (state[0]) {
        lr_state = -127;
    }
    if (state[1]) {
        ud_state = -127;
    }
    if (state[2]) {
        ud_state = 127;
    }
    if (state[3]) {
        lr_state = 127;
    }

    if (stick == 0) {
            ud_state = ud_state * -1;
    }

    joystick_set_axis(left_right, lr_state);
    joystick_set_axis(up_down, ud_state);
}

void handle_dpad(bool *state) {
    int lr_state = 0;
    int ud_state = 0;
    if (state[0]) {
        lr_state = -1;
    }
    if (state[1]) {
        ud_state = -1;
    }
    if (state[2]) {
        ud_state = 1;
    }
    if (state[3]) {
        lr_state = 1;
    }

    if (lr_state == -1) {
        register_joystick_button(15);
        unregister_joystick_button(17);
    } else if (lr_state == 1) {
        unregister_joystick_button(15);
        register_joystick_button(17);
    } else {
        unregister_joystick_button(15);
        unregister_joystick_button(17);
    }

    if (ud_state == -1) {
        register_joystick_button(16);
        unregister_joystick_button(18);
    } else if (ud_state == 1) {
        unregister_joystick_button(16);
        register_joystick_button(18);
    } else {
        unregister_joystick_button(16);
        unregister_joystick_button(18);
    }
}

void handle_button(bool pressed, int button) {
    if (pressed) {
        register_joystick_button(button);
    } else {
        unregister_joystick_button(button);
    }
}

bool handle_universal(bool pressed, int direction) {
    switch (global_saved_values.dir_mode) {
        case 0:
            handle_dpad(handle_socd(pressed, DP_STATE, direction));
            return true;
        case 1:
            handle_stick(0, handle_socd(pressed, LS_STATE, direction));
            return true;
        case 2:
            handle_stick(1, handle_socd(pressed, RS_STATE, direction));
            return true;
    }
    return false;
}

bool process_gamepad(uint16_t keycode, bool pressed) {
    switch (keycode) {
        case GC_SQU:
            handle_button(pressed, 0);
            return false;
        case GC_CRO:
            handle_button(pressed, 1);
            return false;
        case GC_CIR:
            handle_button(pressed, 2);
            return false;
        case GC_TRI:
            handle_button(pressed, 3);
            return false;
        case GC_L1:
            handle_button(pressed, 4);
            return false;
        case GC_R1:
            handle_button(pressed, 5);
            return false;
        case GC_L2:
            handle_button(pressed, 6);
            return false;
        case GC_R2:
            handle_button(pressed, 7);
            return false;
        case GC_SEL:
            handle_button(pressed, 8);
            return false;
        case GC_STA:
            handle_button(pressed, 9);
            return false;
        case GC_L3:
            handle_button(pressed, 10);
            return false;
        case GC_R3:
            handle_button(pressed, 11);
            return false;
        case GC_HOM:
            handle_button(pressed, 12);
            return false;
        case GC_UNL:
            handle_universal(pressed, 0);
            return false;
        case GC_UND:
            handle_universal(pressed, 1);
            return false;
        case GC_UNU:
            handle_universal(pressed, 2);
            return false;
        case GC_UNR:
            handle_universal(pressed, 3);
            return false;
        case GC_DNL:
            handle_dpad(handle_socd(pressed, DP_STATE, 0));
            return false;
        case GC_DND:
            handle_dpad(handle_socd(pressed, DP_STATE, 1));
            return false;
        case GC_DNU:
            handle_dpad(handle_socd(pressed, DP_STATE, 2));
            return false;
        case GC_DNR:
            handle_dpad(handle_socd(pressed, DP_STATE, 3));
            return false;
        case GC_LNL:
            handle_stick(0, handle_socd(pressed, LS_STATE, 0));
            return false;
        case GC_LND:
            handle_stick(0, handle_socd(pressed, LS_STATE, 1));
            return false;
        case GC_LNU:
            handle_stick(0, handle_socd(pressed, LS_STATE, 2));
            return false;
        case GC_LNR:
            handle_stick(0, handle_socd(pressed, LS_STATE, 3));
            return false;
        case GC_RNL:
            handle_stick(1, handle_socd(pressed, RS_STATE, 0));
            return false;
        case GC_RND:
            handle_stick(1, handle_socd(pressed, RS_STATE, 1));
            return false;
        case GC_RNU:
            handle_stick(1, handle_socd(pressed, RS_STATE, 2));
            return false;
        case GC_RNR:
            handle_stick(1, handle_socd(pressed, RS_STATE, 3));
            return false;
        case GC_TOG:
            if (pressed) {
                change_dir_mode(-1);
                clear_button_axis();
            }
            return false;
        case GC_TDP:
            if (pressed) {
                change_dir_mode(0);
                clear_button_axis();
            }
            return false;
        case GC_TLS:
            if (pressed) {
                change_dir_mode(1);
                clear_button_axis();
            }
            return false;
        case GC_TRS:
            if (pressed) {
                change_dir_mode(2);
                clear_button_axis();
            }
            return false;
        case GC_SCD:
            if (pressed) {
                change_socd_mode(-1);
                clear_button_axis();
            }
            return false;
        case GC_UPLI:
            if (pressed) {
                change_socd_mode(0);
                clear_button_axis();
            }
            return false;
        case GC_LAST:
            if (pressed) {
                change_socd_mode(1);
                clear_button_axis();
            }
            return false;
        case GC_SUPN:
            if (pressed) {
                change_socd_mode(2);
                clear_button_axis();
            }
            return false;
        case GC_NEUT:
            if (pressed) {
                change_socd_mode(3);
                clear_button_axis();
            }
            return false;
    }
    return true;
};
