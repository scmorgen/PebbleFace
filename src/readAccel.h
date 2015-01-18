#pragma once
#ifndef READ_ACCEL_H
#define READ_ACCEL_H


#define GRAPH_WIDTH 139
#define GRAPH_HEIGHT 90
#define ACCEL_STEP_MS 50
#define NUM_ACCEL_SAMPLES 1
#define ACCEL_SAMPLING_RATE ACCEL_SAMPLING_10HZ 

extern int accel_x[GRAPH_WIDTH];
extern int accel_y[GRAPH_WIDTH];
extern int accel_z[GRAPH_WIDTH];
extern Layer *bargraph_layer;
  
void init_readAccel();
void deinit_readAccel();
void set_up_bargraph(Window * window);

//***************Special Notes to Enabling it in graphics.c********///
//Add #include <readAccel.h> to graphics.c
// Add  set_up_bargraph(window); to windows_load in graphics.c
// Add layer_destroy(bargraph_layer); to window_unload in graphics.c

#endif