#include <pebble.h>
#include "menu_layer.h"
  
// setup a Window using a pointer
static Window *s_main_window;

// setup a TextLayer using a pointer
static TextLayer *s_text_layer;

//---the click event handler for the SELECT button---
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  menu_layer_push();
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

static void main_window_load(Window *window){
    // create a company TextLayer
    s_text_layer = text_layer_create(GRect(0, 55, 144, 50));
    text_layer_set_background_color(s_text_layer, GColorClear);
    text_layer_set_text_color(s_text_layer, GColorBlack);
    text_layer_set_text(s_text_layer, "HPQ");
    
    // improve the layout to be more like a watchface
    text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
    text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
    
    // add it as a child layer to the Window's root layer
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_text_layer));
}

static void main_window_unload(Window *window){
    // destroy TextLayer
    text_layer_destroy(s_text_layer);
    
}

static void init() {
    // create main Window element and assign to pointer
    s_main_window = window_create();
	
		window_set_background_color(s_main_window, GColorJaegerGreen);
  
    window_set_click_config_provider(s_main_window, click_config_provider);
    
    // set handlers to manage the elements inside the window
    window_set_window_handlers(s_main_window, (WindowHandlers){
        .load = main_window_load,
        .unload = main_window_unload
    });
    
    // show the Window on the watch, with animated=true
    window_stack_push(s_main_window, true);
    
}

static void deinit() {
    // destroy Window
    window_destroy(s_main_window);
}

int main (void){
    init();
    app_event_loop();
    deinit();
}