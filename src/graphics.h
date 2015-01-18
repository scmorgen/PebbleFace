#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

//Text Display variables
extern Window *window;
extern TextLayer *phase_layer, *score_layer, *role_layer, *gesture_layer;
extern char phase_buffer[64], score_buffer[32], role_buffer[32], gesture_buffer[32];

//Functions
void init_window();
void deinit_window();

#endif