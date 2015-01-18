#include <pebble.h>
#include <graphics.h>
#include <Messaging.h>
#include <readAccel.h>
  
  //*******************Main Body*********/
 
static void init(void) {
  init_window();
  init_messaging();
  init_readAccel();
}
 
static void deinit(void) {
  deinit_readAccel();
  deinit_messaging();
  deinit_window();
}
 
int main(void) {
  init();
 
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);
 
  app_event_loop();
  deinit();
}