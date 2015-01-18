#include <pebble.h>
#include "graphics.h"
#include "readAccel.h"

Window *window;
TextLayer *phase_layer, *score_layer, *role_layer, *gesture_layer;
char phase_buffer[64], score_buffer[32], role_buffer[32], gesture_buffer[32];

#define PEBBLE_WIDTH 144
#define PEBBLE_HEIGHT 168
#define TEXT_HEIGHT 30

/*************Private Function Declarations *******/
static void window_unload(Window *window);
static void window_load(Window *window);
static void set_up_text_layer(Window * window, TextLayer * text_layer, int y, char* text, GTextAlignment alignment, char * aFont);
static TextLayer * init_text_layer(GRect location, GTextAlignment alignment, char * aFont);

/*************Public Functions ***************/
//Initializes the window
void init_window() {
  window = window_create();
  //window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

//deinitializes the window
void deinit_window() { 
  window_destroy(window);
}

/************Private Functions *************/

//destroys the screen at the end
static void window_unload(Window *window) {
  //destroy all elements here
  layer_destroy(bargraph_layer);
  
  text_layer_destroy(phase_layer);
  text_layer_destroy(score_layer);
  text_layer_destroy(role_layer);
  text_layer_destroy(gesture_layer);
}

//loads the screen
static void window_load(Window *window) {
  set_up_bargraph(window);
  set_up_text_layer(window, phase_layer, 0, "INIT", GTextAlignmentLeft, FONT_KEY_GOTHIC_14);
  set_up_text_layer(window, score_layer, 0, "", GTextAlignmentRight, FONT_KEY_GOTHIC_14);
  set_up_text_layer(window, role_layer, (PEBBLE_HEIGHT-TEXT_HEIGHT)/2, "", GTextAlignmentCenter, FONT_KEY_GOTHIC_24_BOLD);
  set_up_text_layer( window, gesture_layer, (PEBBLE_HEIGHT-TEXT_HEIGHT), "Last Gesture: N/A", GTextAlignmentCenter, FONT_KEY_GOTHIC_18);
  
}

//sets up the text layer and places it on the window
static void set_up_text_layer(Window * window, TextLayer * text_layer, int y, char * text, GTextAlignment alignment, char * aFont) {
  text_layer = init_text_layer(GRect(3, y,PEBBLE_WIDTH-6, TEXT_HEIGHT), alignment, aFont);
  text_layer_set_text(text_layer, text);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));
}

//initializes all the variables for the text layer
static TextLayer* init_text_layer(GRect location, GTextAlignment alignment, char * aFont) {
  TextLayer *layer = text_layer_create(location);
  text_layer_set_text_color(layer, GColorBlack);
  text_layer_set_background_color(layer, GColorClear);
  text_layer_set_font(layer, fonts_get_system_font(aFont));
  text_layer_set_text_alignment(layer, alignment);
  return layer;
}