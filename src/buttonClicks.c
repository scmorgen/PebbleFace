#include <pebble.h>
#include "buttonClicks.h"
#include <graphics.h>
#include <Messaging.h>

/************Private Declarations ************/
static void select_click_handler(ClickRecognizerRef recognizer, void *context);
static void up_click_handler(ClickRecognizerRef recognizer, void *context);
static void down_click_handler(ClickRecognizerRef recognizer, void *context);

/*******************Public Functions********************/
void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}
  
/******************Private Functions*****************/
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(gesture_layer, "Last Gesture: SELECT");
  sendGesture(DOWN);
}
 
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(gesture_layer, "Last Gesture: UP");
  sendGesture(UP);
}
 
static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(gesture_layer, "Last Gesture: DOWN");
  sendGesture(SELECT);
  
}

//
