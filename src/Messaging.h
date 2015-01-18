#pragma once
#ifndef MESSAGING_H
#define MESSAGING_H

//out messages
enum {
    KEY_GESTURE = 0,
    JAZZ_HANDS = 1,  // Jazz Hands
    POKE = 2,  //Poke
    RAISE_THE_ROOF = 3,  //Raise the Roof 
    RUNNING= 4, //running
    DOWN=5, //Down Button
    UP= 6, //Up button
    SELECT= 7,
};

//in messages
enum {
    KEY_SEND_ROLE= 8,
    KEY_SEND_PHASE=9,
    KEY_SCORE_UPDATE= 10,
    WAITING_ROOM_SCREEN= 11,
    GAME_PLAY_SCREEN=12,
    FINAL_SCREEN=13
};

extern int lastMessage;


//send a gesture out to the phone
void init_messaging();
void deinit_messaging();
void sendGesture(int gestureKey);

#endif