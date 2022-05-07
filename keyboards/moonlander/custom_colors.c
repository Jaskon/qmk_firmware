// Custom colors

#define LAYER_COUNT 8
#define MAX_KEYS_OF_ONE_TYPE 4

#include <memory.h>
typedef struct KEY_POS {
  uint8_t layer;
  uint8_t index;
} KEY_POS;

KEY_POS nkro_positions[MAX_KEYS_OF_ONE_TYPE];
uint8_t nkro_positions_count = 0;
KEY_POS change_os_positions[4];
uint8_t change_os_positions_count = 0;


void __set_key_color(uint8_t index, uint8_t h, uint8_t s, uint8_t v) {
  HSV hsv = { .h = h, .s = s, .v = v };
  RGB rgb = hsv_to_rgb(hsv);
  float f = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
  rgb_matrix_set_color(index, f * rgb.r, f * rgb.g, f * rgb.b);
}

uint8_t __get_key_index(uint8_t row, uint8_t col) {
  return col * 5 + row;
}

void _set_nkro_color(uint8_t layer) {
  for (uint8_t i = 0; i < nkro_positions_count; i++) {
    if (layer == nkro_positions[i].layer) {
      if (keymap_config.nkro) {
        __set_key_color(nkro_positions[i].index, 80, 255, 255);  // green
      } else {
        __set_key_color(nkro_positions[i].index, 33, 255, 255);  // orange
      }
    }
  }
}

void _set_change_os_color(uint8_t layer) {
  for (uint8_t i = 0; i < change_os_positions_count; i++) {
    if (layer == change_os_positions[i].layer) {
      if (current_os == WINDOWS) {
        __set_key_color(change_os_positions[i].index, 80, 255, 255);  // green
      } else {
        __set_key_color(change_os_positions[i].index, 33, 255, 255);  // orange
      }
    }
  }
}


void process_custom_colors(void) {
    uint8_t layer = biton32(layer_state);
    _set_nkro_color(layer);
    _set_change_os_color(layer);
}

void init_key_indexes_to_color(void) {
    nkro_positions_count = 0;
    change_os_positions_count = 0;
    memset(nkro_positions, 0, sizeof nkro_positions);
    memset(change_os_positions, 0, sizeof change_os_positions);

    for (uint8_t layer = 0; layer < LAYER_COUNT; layer++) {
        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
            for (uint8_t col = 0; col < MATRIX_COLS; col++) {
                if (keymaps[layer][row][col] == MAGIC_TOGGLE_NKRO) {
                    KEY_POS k = {.layer = layer, .index = __get_key_index(row, col)};
                    nkro_positions[nkro_positions_count] = k;
                    nkro_positions_count++;
                }

                if (keymaps[layer][row][col] == KC_F24) {
                    KEY_POS k = {.layer = layer, .index = __get_key_index(row, col)};
                    change_os_positions[nkro_positions_count] = k;
                    change_os_positions_count++;
                }
            }
        }
    }
}
