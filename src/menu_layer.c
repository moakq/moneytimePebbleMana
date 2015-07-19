#include <pebble.h>

#include "menu_layer.h"
#include "pin_window.h"
#include "ignored.h"
  
// global pointer to MenuLayer
MenuLayer *menu_layer;

Window* window;

void draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context)
{
    // which row is it?
    switch(cell_index->row)
    {
    case 0:
        menu_cell_basic_draw(ctx, cell_layer, "Buy", "", NULL);
        break;
    case 1:
        menu_cell_basic_draw(ctx, cell_layer, "Ignore", "", NULL);
        break;
    case 2:
        menu_cell_basic_draw(ctx, cell_layer, "Sell", "", NULL);
        break;
    }
}

static void pin_complete_callback(PIN pin, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Pin was %d %d %d", pin.digits[0], pin.digits[1], pin.digits[2]);
  pin_window_pop((PinWindow*)context, true);
}

void select_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
  switch(cell_index->row) {
		case 0:
				{
					PinWindow *pin_window = pin_window_create((PinWindowCallbacks) {
							.pin_complete = pin_complete_callback
						});
						pin_window_push(pin_window, true);
				}
			break;
		case 1:
			ignored_push();
			break;
		case 2:
					{
					PinWindow *pin_window = pin_window_create((PinWindowCallbacks) {
							.pin_complete = pin_complete_callback
						});
						pin_window_push(pin_window, true);
					}
			break;
	}
}
 
uint16_t num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{
   return 3;
}
 
void window_load(Window *window)
{
    // create it - 12 is approx height of the top bar
    menu_layer = menu_layer_create(GRect(0, 0, 144, 168 - 16));
 
    // let it receive clicks
    menu_layer_set_click_config_onto_window(menu_layer, window);
 
    // give it its callbacks
    MenuLayerCallbacks callbacks = {
        .draw_row = (MenuLayerDrawRowCallback) draw_row_callback,
        .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback) num_rows_callback,
        .select_click = (MenuLayerSelectCallback) select_callback
    };
    menu_layer_set_callbacks(menu_layer, NULL, callbacks);
 
    // add to Window
    layer_add_child(window_get_root_layer(window), menu_layer_get_layer(menu_layer));
}
 
void window_unload(Window *window)
{
    // destroy menu_layer
    menu_layer_destroy(menu_layer);
}
 
void menu_layer_push() {
  if(!window) {
    window = window_create();
		window_set_background_color(window, GColorJaegerGreen);
    window_set_window_handlers(window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
  }
  window_stack_push(window, true);
}