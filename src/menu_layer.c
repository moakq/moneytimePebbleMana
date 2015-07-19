#include <pebble.h>

#include "menu_layer.h"
  
// global pointer to MenuLayer
MenuLayer *menu_layer;

Window* window;



void draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context)
{
    // which row is it?
    switch(cell_index->row)
    {
    case 0:
        menu_cell_basic_draw(ctx, cell_layer, "1. Buy", "", NULL);
        break;
    case 1:
        menu_cell_basic_draw(ctx, cell_layer, "2. Ignore", "", NULL);
        break;
    case 2:
        menu_cell_basic_draw(ctx, cell_layer, "3. Sell", "", NULL);
        break;
    }
}
 
uint16_t num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{
   return 3;
}

void select_click_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context)
{
    // get which row
    int which = cell_index->row;
 
    //The array that will hold the on/off vibration times
    uint32_t segments[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
 
    // build the pattern (milliseconds on and off in alternating positions)
    for(int i = 0; i < which + 1; i++)
    {
        segments[2 * i] = 200;
        segments[(2 * i) + 1] = 100;
    }
 
    // create a VibePattern data structure
    VibePattern pattern = {
        .durations = segments,
        .num_segments = 16
    };
 
    // do the vibration pattern!
    vibes_enqueue_custom_pattern(pattern);
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
        .select_click = (MenuLayerSelectCallback) select_click_callback
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
    window_set_window_handlers(window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
  }
  window_stack_push(window, true);
}