// Custom keys

// Enum with custom keycodes is in 'moonlander.h'
// Defines with keycodes signature is in 'moonlander.h' also

static uint16_t last_key_timer;
uint16_t last_key_pressed = false;
bool layer_was_on[16];

enum OS {
    WINDOWS,
    MACOS
};
static enum OS current_os = WINDOWS;

void custom_keys_init(void) {
    for (uint8_t i = 0; i < 16; i++) {
        layer_was_on[i] = false;
    }
}

void _process_TTI(uint8_t layer_num, uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        // Key down
        if (IS_LAYER_ON(layer_num)) {
            layer_was_on[layer_num] = true;
            return;
        }
        last_key_timer = timer_read();
        layer_on(layer_num);
        return;
    }

    // Key up
    if (layer_was_on[layer_num]) {
        layer_was_on[layer_num] = false;
        layer_off(layer_num);
        return;
    }
    if (last_key_pressed == keycode && timer_elapsed(last_key_timer) < TAPPING_TERM) {
        return;
    }
    layer_off(layer_num);
}

void _process_LTI(uint8_t layer_num, uint16_t lt_keycode, uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        // Key down
        last_key_timer = timer_read();
        layer_on(layer_num);
        return;
    }

    // Key up
    if (last_key_pressed == keycode && timer_elapsed(last_key_timer) < TAPPING_TERM) {
        tap_code(lt_keycode);
    }
    layer_off(layer_num);
}

void _process_change_os(keyrecord_t *record) {
    if (record->event.pressed) {
        current_os = current_os == WINDOWS ? MACOS : WINDOWS;
    }
}

void _process_ctrl_left_macos(keyrecord_t *record) {
    if (record->event.pressed) {
        return register_code16(LALT(KC_LEFT));
    }
    unregister_code16(LALT(KC_LEFT));
}

void _process_ctrl_right_macos(keyrecord_t *record) {
    if (record->event.pressed) {
        return register_code16(LALT(KC_RIGHT));
    }
    unregister_code16(LALT(KC_RIGHT));
}

void _process_ctrl_backspace_macos(keyrecord_t *record) {
  if (record->event.pressed) {
    return register_code16(LALT(KC_BSPACE));
  }
  unregister_code16(LALT(KC_BSPACE));
}

void _process_ctrl_z_macos(keyrecord_t *record) {
  if (record->event.pressed) {
    return register_code16(LGUI(KC_Z));
  }
  unregister_code16(LGUI(KC_Z));
}

void _process_ctrl_shift_z_macos(keyrecord_t *record) {
  if (record->event.pressed) {
    return register_code16(LGUI(LSFT(KC_Z)));
  }
  unregister_code16(LGUI(LSFT(KC_Z)));
}

bool process_custom_keys(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        last_key_pressed = keycode;
    }

    if (keycode >= TTI_SAFE_RANGE && keycode < LTI_SAFE_RANGE) {
        _process_TTI(keycode - TTI_SAFE_RANGE, keycode, record);
        return true;
    }

    if (keycode >= LTI_SAFE_RANGE && keycode < MY_NEW_SAFE_RANGE) {
        _process_LTI((keycode - LTI_SAFE_RANGE) / 256, (keycode - LTI_SAFE_RANGE) % 256, keycode, record);
        return true;
    }

    if (keycode == KC_F24) {
        _process_change_os(record);
        return true;
    }

    if (current_os == MACOS) {
        if (keycode == LCTL(KC_LEFT) || keycode == RCTL(KC_LEFT)) {
            _process_ctrl_left_macos(record);
            return true;
        }

        if (keycode == LCTL(KC_RIGHT) || keycode == RCTL(KC_RIGHT)) {
            _process_ctrl_right_macos(record);
            return true;
        }

        if (keycode == LCTL(KC_BSPACE) || keycode == RCTL(KC_BSPACE)) {
            _process_ctrl_backspace_macos(record);
        }

        if (keycode == LCTL(KC_Z) || keycode == RCTL(KC_Z)) {
          _process_ctrl_z_macos(record);
        }

        if (keycode == LCTL(LSFT(KC_Z)) || keycode == RCTL(LSFT(KC_Z))) {
          _process_ctrl_shift_z_macos(record);
        }
    }

    return false;
}
