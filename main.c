#include <pebble.h>

// setup a Window using a pointer
static Window *s_main_window;

// setup a TextLayer using a pointer
static TextLayer *s_time_layer;

// update time
static void update_time() {
    // get a tm structure
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);
    
    // create a long-lived buffer
    static char buffer[] = "00:00";
    
    // write the current hours and minutes into the buffer
    if(clock_is_24h_style() == true) {
        // use 24 hour format
        strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
    } else {
        // use 12 hour format
        strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
    }
    
    // display this time on the TextLayer
    text_layer_set_text(s_time_layer, buffer);
}

// gives the service to call whenever the time changes
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    update_time();
}

static void main_window_load(Window *window){
    // create time TextLayer
    s_time_layer = text_layer_create(GRect(0, 55, 144, 50));
    text_layer_set_background_color(s_time_layer, GColorClear);
    text_layer_set_text_color(s_time_layer, GColorBlack);
    // not needed anymore since we are updating the time
    //text_layer_set_text(s_time_layer, "00:00");
    
    // improve the layout to be more like a watchface
    text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
    text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
    
    // add it as a child layer to the Window's root layer
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window){
    // destroy TextLayer
    text_layer_destroy(s_time_layer);
    
}

static void init() {
    // create main Window element and assign to pointer
    s_main_window = window_create();
    
    // set handlers to manage the elements inside the window
    window_set_window_handlers(s_main_window, (WindowHandlers){
        .load = main_window_load,
        .unload = main_window_unload
    });
    
    // show the Window on the watch, with animated=true
    window_stack_push(s_main_window, true);
    
    // make sure the time is displayed from the start
    update_time();
    
    // register with TickTimerService
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
    
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