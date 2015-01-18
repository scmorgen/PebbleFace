#include <pebble.h>
#include <graphics.h>
#include "Messaging.h"

int lastMessage;
/************Private Declarations ************/
static void reactToMessages(int key, int value, char * string_value);
  
static void inbox_dropped_callback(AppMessageResult reason, void *context);
static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context);
static void outbox_sent_callback(DictionaryIterator *iterator, void *context);
static void in_received_handler(DictionaryIterator *received, void *context);
static void send_int(uint8_t key, uint8_t cmd);

/**************Public Functions ********************/
//initiates messaging by setting up message handlers
void init_messaging() {
  app_message_register_inbox_received(in_received_handler);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  app_message_open(512, 512);
}

//deinitiates messaging by deallocing the messages
void deinit_messaging() {
  app_message_deregister_callbacks();
}

//handles sending a gesture
void sendGesture(int gestureKey) {
  if (gestureKey==-1) //for case where gesture is not sent
    send_int(KEY_GESTURE, lastMessage);
  else {
    lastMessage= gestureKey;
    send_int(KEY_GESTURE, gestureKey);
  } 
}

/*****************Private Functions ***********************/
//Take actions upong receiving messages
static void reactToMessages(int key, int value, char * string_value) {
  switch(key) {
    case KEY_SEND_PHASE: //Location received  
      switch(value) {
        case WAITING_ROOM_SCREEN:
          text_layer_set_text(phase_layer, "Waiting Room");
          text_layer_set_text(role_layer, "");
          text_layer_set_text(gesture_layer, "Last Gesture: N/A");
          text_layer_set_text(score_layer, "");
          break;         
        case GAME_PLAY_SCREEN:
          text_layer_set_text(phase_layer, "Play Screen");
          break;       
        case FINAL_SCREEN:
          text_layer_set_text(phase_layer, "Its the end!");
          text_layer_set_text(gesture_layer, "Last Gesture: N/A");
          break;
      }
      break;
    
  case KEY_SEND_ROLE: //Role received
      snprintf(role_buffer, sizeof("a really long role name"), "%s", string_value);
      text_layer_set_text(role_layer, (char*) &role_buffer);
      break;
    
  case KEY_SCORE_UPDATE:
      snprintf(score_buffer, sizeof("Score: XXX"), "Score: %d", value);
      text_layer_set_text(score_layer, (char*) &score_buffer);
    
      if ((value < 0) ||  (value >= 100)) {
        if (value>=100) strcpy(string_value,"You Win!");
        else strcpy(string_value, "You lose!");
        snprintf(role_buffer, sizeof("a really long role name"), "%s", string_value);
        text_layer_set_text(role_layer, (char*) &role_buffer);
      }
    
      break;
    break;
  }
} 

//Callback if pebble received message but it sent back a 'nack'
static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

//Callback if pebble sent message but it failed. Here we resend the prior message
static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
  sendGesture(-1);
}

//Callback if pebble sent message and it was successful
static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

// Callback if pebble received message successfully
// Processes incoming messages
static void in_received_handler(DictionaryIterator *received, void *context) {
	(void) context;
	Tuple * t = dict_read_first(received);
	while(t !=NULL) {
    
    //Get key
    int key = t->key;
    //Get integer value, if present
    int value = t->value->int32;
    //Get string value, if present
    char string_value[32];
    strcpy(string_value, t->value->cstring);
    reactToMessages(key, value, string_value);
    
    t = dict_read_next(received);
	}
}

//Sends int from pebble to phone
void send_int(uint8_t key, uint8_t cmd) {
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
      
    Tuplet value = TupletInteger(key, cmd);
    dict_write_tuplet(iter, &value);
      
    app_message_outbox_send();
}