#include QMK_KEYBOARD_H

#define NO_ACTION_TAPPING

#ifdef BONGO_ENABLE
#include "bongo.h"
#endif

enum layer_names {
  _BASE,
  _VIA1,
  _VIA2,
  _VIA3
};

#define _BASE     0
#define _VIA1     1
#define _VIA2     2
#define _VIA3     3

bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;

bool numlock_set = false;

enum custom_keycodes {
    PROG = SAFE_RANGE,
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT(
           KC_ASTR, KC_SLSH, KC_SLSH, 
  RGB_TOG, KC_9, KC_6, KC_3, 
  KC_HOME, KC_8, KC_5, KC_2, 
  KC_MPLY, KC_7, KC_4, KC_1, 
  KC_MUTE, KC_PLUS, KC_MINS, KC_0 
  ),
  [_VIA1] = LAYOUT(
           KC_NO, KC_NO, KC_NO,
    KC_NO, KC_NO, KC_NO, KC_NO,
    KC_NO, KC_NO, KC_NO, KC_NO,
    KC_NO, KC_NO, KC_NO, KC_NO,
    KC_NO, KC_NO, KC_NO, KC_NO
  ),
  [_VIA2] = LAYOUT(
           KC_NO, KC_NO, KC_NO,
    KC_NO, KC_NO, KC_NO, KC_NO,
    KC_NO, KC_NO, KC_NO, KC_NO,
    KC_NO, KC_NO, KC_NO, KC_NO,
    KC_NO, KC_NO, KC_NO, KC_NO
  ),
  [_VIA3] = LAYOUT(
           KC_NO, KC_NO, KC_NO,
    KC_NO, KC_NO, KC_NO, KC_NO,
    KC_NO, KC_NO, KC_NO, KC_NO,
    KC_NO, KC_NO, KC_NO, KC_NO,
    KC_NO, KC_NO, KC_NO, KC_NO
  ),
};



/*===========================================    OLED CONFIGURATION    ===========================================*/

// Layer names:  Should be exactly 5 characters in length if vertical display, or 6 characters if horizontal
#define MA_LAYER_NAME     "Base  "      // Layer _MA name
#define L1_LAYER_NAME     "Func  "      // Layer _L1 name
#define L2_LAYER_NAME     "Ext: 1"      // Layer _L2 name
#define L3_LAYER_NAME     "Ext: 2"      // Layer _L3 name

/*================================================================================================================*/

// Write active layer name
static void render_layer_state(void) {
  switch (get_highest_layer(layer_state)) {
  case _BASE:
      oled_write_P(PSTR(MA_LAYER_NAME), false);
      break;
  case _VIA1:
      oled_write_P(PSTR(L1_LAYER_NAME), false);
      break;
  case _VIA2:
      oled_write_P(PSTR(L2_LAYER_NAME), false);
      break;
  case _VIA3:
      oled_write_P(PSTR(L3_LAYER_NAME), false);
      break;
  default:
      oled_write("ERROR", false);
      break;
  }
}

#ifdef OLED_ENABLE

void oled_task_user(void) {
    draw_bongo();
    render_layer_state();
}
#endif


void matrix_init_user(void) { matrix_init_remote_kb(); }

void matrix_scan_user(void) { 
    matrix_scan_remote_kb(); 
    if (is_alt_tab_active) {
        if (timer_elapsed(alt_tab_timer) > 1250) {
        unregister_code(KC_LALT);
        is_alt_tab_active = false;
        }
    }
}
    

uint8_t selected_layer = 0;
void encoder_update_user(uint8_t index, bool clockwise) {
  switch (index) {
    case 0:
        switch (get_highest_layer(layer_state)) {
            case _VIA3:
                if (clockwise) {
                    tap_code(KC_KP_1);
                } else {
                    tap_code(KC_KP_2);
                }
                break;
            case _VIA2:
                if (clockwise) {
                    tap_code(KC_KP_3);
                } else {
                    tap_code(KC_KP_4);
                }
                break;
            case _VIA1:
                if (clockwise) {
                    rgblight_increase_val();
                } else {
                    rgblight_decrease_val();
                }
                break;
            default:
                if (clockwise) {
                    tap_code(KC_VOLU);
                } else {
                    tap_code(KC_VOLD);
                }
                break;
        }
        break;
    case 1:
        switch (get_highest_layer(layer_state)) {
            case _VIA3:
                if (clockwise) {
                    tap_code(KC_KP_1);
                } else {
                    tap_code(KC_KP_2);
                }
                break;
            case _VIA2:
                if (clockwise) {
                    tap_code(KC_KP_3);
                } else {
                    tap_code(KC_KP_4);
                }
                break;
            case _VIA1:
                if (clockwise) {
                    rgblight_decrease_sat();
                } else {
                    rgblight_increase_sat();
                }
                break;
            default:
                if (clockwise) {
                    tap_code(KC_VOLU);
                } else {
                    tap_code(KC_VOLD);
                }
                break;
        }
        break;
    case 2:
        switch (get_highest_layer(layer_state)) {
            case _VIA3:
                if (clockwise) {
                    tap_code(KC_KP_1);
                } else {
                    tap_code(KC_KP_2);
                }
                break;
            case _VIA2:
                if (clockwise) {
                    tap_code(KC_KP_3);
                } else {
                    tap_code(KC_KP_4);
                }
                break;
            case _VIA1:
                if (clockwise) {
                    rgblight_increase_hue();
                } else {
                    rgblight_decrease_hue();
                }
                break;
            default:
                if (clockwise) {
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;
                    register_code(KC_LALT);
                }
                alt_tab_timer = timer_read();
                tap_code16(KC_TAB);
                } else {
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;
                    register_code(KC_LALT);
                }
                alt_tab_timer = timer_read();
                tap_code16(S(KC_TAB));
                }
                break;
        }
        break;
    case 3:
        if (clockwise&& selected_layer  < 10) {
            selected_layer ++;
        } else if (!clockwise && selected_layer  > 0){
            selected_layer --;
        }
        layer_clear();
        layer_on(selected_layer);
        break;
  }
}