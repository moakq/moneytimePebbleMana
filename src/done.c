#include <pebble.h>
#include "done.h"
#include "menu_layer.h"

// setup a Window using a pointer
Window *s_main_window;

// setup a TextLayer using a pointer
TextLayer *s_text_layer;

void main_window_load(Window *window){
    // create a company TextLayer
    s_text_layer = text_layer_create(GRect(0, 55, 144, 50));
    text_layer_set_background_color(s_text_layer, GColorClear);
    text_layer_set_text_color(s_text_layer, GColorWhite);
    text_layer_set_text(s_text_layer, "Done!");
    
    // improve the layout to be more like a watchface
    text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
    text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
    
    // add it as a child layer to the Window's root layer
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_text_layer));
}

void main_window_unload(Window *window){
    // destroy TextLayer
    text_layer_destroy(s_text_layer);
}

void done_push() {
  if(!s_main_window) {
    s_main_window = window_create();
		window_set_background_color(s_main_window, GColorCyan);
    window_set_window_handlers(s_main_window, (WindowHandlers) {
        .load = main_window_load,
        .unload = main_window_unload,
    });
  }
  window_stack_push(s_main_window, true);
}