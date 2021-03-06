#include <pebble.h>
#include "pin_window.h"
#include "selection_layer.h"
#include "done.h"
	
static char* selection_handle_get_text(int index, void *context) {
  PinWindow *pin_window = (PinWindow*)context;
  snprintf(
    pin_window->field_buffs[index], 
    sizeof(pin_window->field_buffs[0]), "%d",
    (int)pin_window->pin.digits[index]
  );
  return pin_window->field_buffs[index];
}

static void selection_handle_complete(void *context) {
	done_push();
}

static void selection_handle_inc(int index, uint8_t clicks, void *context) {
  PinWindow *pin_window = (PinWindow*)context;
  pin_window->pin.digits[index]++;
}

static void selection_handle_dec(int index, uint8_t clicks, void *context) {
  PinWindow *pin_window = (PinWindow*)context;
  pin_window->pin.digits[index]--;
  if(pin_window->pin.digits[index] < 0) {
    pin_window->pin.digits[index] = MAX_VALUE;
  }
}

PinWindow* pin_window_create(PinWindowCallbacks callbacks) {
  PinWindow *pin_window = (PinWindow*)malloc(sizeof(PinWindow));
  if (pin_window) {
    pin_window->window = window_create();
    pin_window->callbacks = callbacks;
    if (pin_window->window) {
      pin_window->field_selection = 0;
      for(int i = 0; i < NUM_CELLS; i++) {
        pin_window->pin.digits[i] = 0;
      }
      
      // Get window parameters
      Layer *window_layer = window_get_root_layer(pin_window->window);
      GRect bounds = layer_get_bounds(window_layer);
      
      // Main TextLayer
#ifdef PBL_SDK_3
      pin_window->main_text = text_layer_create(GRect(0, 30, bounds.size.w, 40));
#else
      pin_window->main_text = text_layer_create(GRect(0, 15, bounds.size.w, 40));
#endif
      text_layer_set_text(pin_window->main_text, "How many?");
      text_layer_set_font(pin_window->main_text, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
      text_layer_set_text_alignment(pin_window->main_text, GTextAlignmentCenter);
      layer_add_child(window_layer, text_layer_get_layer(pin_window->main_text));
      
      // Sub TextLayer
#ifdef PBL_SDK_3
      pin_window->sub_text = text_layer_create(GRect(1, 125, bounds.size.w, 40));
#else
      pin_window->sub_text = text_layer_create(GRect(1, 110, bounds.size.w, 40));
#endif

      // Create selection layer
#ifdef PBL_SDK_3
      pin_window->selection = selection_layer_create(GRect(8, 75, 128, 34), NUM_CELLS);
#else
      pin_window->selection = selection_layer_create(GRect(8, 60, 128, 34), NUM_CELLS);
#endif
      for (int i = 0; i < NUM_CELLS; i++) {
        selection_layer_set_cell_width(pin_window->selection, i, 40);
      }
      selection_layer_set_cell_padding(pin_window->selection, 4);
#ifdef PBL_COLOR
      selection_layer_set_active_bg_color(pin_window->selection, GColorRed);
      selection_layer_set_inactive_bg_color(pin_window->selection, GColorDarkGray);
#endif
      selection_layer_set_click_config_onto_window(pin_window->selection, pin_window->window);
      selection_layer_set_callbacks(pin_window->selection, pin_window, (SelectionLayerCallbacks) {
        .get_cell_text = selection_handle_get_text,
        .complete = selection_handle_complete,
        .increment = selection_handle_inc,
        .decrement = selection_handle_dec,
      });
      layer_add_child(window_get_root_layer(pin_window->window), pin_window->selection);

#ifdef PBL_SDK_3

#endif
      return pin_window;
    }
  }

  APP_LOG(APP_LOG_LEVEL_ERROR, "Failed to create PinWindow");
  return NULL;
}

void pin_window_destroy(PinWindow *pin_window) {
  if (pin_window) {
#ifdef PBL_SDK_3

#endif
    selection_layer_destroy(pin_window->selection);
    text_layer_destroy(pin_window->sub_text);
    text_layer_destroy(pin_window->main_text);
    free(pin_window);
    pin_window = NULL;
    return;
  }
}

void pin_window_push(PinWindow *pin_window, bool animated) {
  window_stack_push(pin_window->window, animated);
}

void pin_window_pop(PinWindow *pin_window, bool animated) {
  window_stack_remove(pin_window->window, animated);
}

bool pin_window_get_topmost_window(PinWindow *pin_window) {
  return window_stack_get_top_window() == pin_window->window;
}

void pin_window_set_highlight_color(PinWindow *pin_window, GColor color) {
  pin_window->highlight_color = color;
  selection_layer_set_active_bg_color(pin_window->selection, color);
}

