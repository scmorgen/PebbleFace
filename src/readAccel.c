#include <pebble.h>
#include "readAccel.h"
#include "gestureRecogn.h"
  
int offset= 30;
int  accel_x[GRAPH_WIDTH];
int accel_y[GRAPH_WIDTH];
int accel_z[GRAPH_WIDTH];
Layer * bargraph_layer;
  
static AppTimer *timer;
static int index=0;
static int curXAccel, curYAccel, curZAccel;


/****************Private Declarations *****************/
static void accel_handler();
static void bargraph_layer_update_callback(Layer *me, GContext *context);
static void draw_bar_graph(GContext* ctx, GPoint origin, GSize size, int* list);
static void timer_callback(void *data);

/*****************Public Functions ********************/
void init_readAccel() {
  //Initiate Timer lister
  timer = app_timer_register(ACCEL_STEP_MS, timer_callback, NULL);  
  //subscribe to accel data service and initiate accelermeter listener
  accel_data_service_subscribe(NUM_ACCEL_SAMPLES, accel_handler);
  accel_service_set_sampling_rate(ACCEL_SAMPLING_RATE);
}

void deinit_readAccel() {
  accel_data_service_unsubscribe();
}

void set_up_bargraph(Window * window) {
  GRect frame = layer_get_frame(window_get_root_layer(window));
  bargraph_layer = layer_create(frame);
  layer_set_update_proc(bargraph_layer, bargraph_layer_update_callback);
  layer_add_child(window_get_root_layer(window), bargraph_layer);
}


/****************Private Functions *********************/
static void accel_handler(AccelData *data, uint32_t num_samples) {
  
  //Capture the new data
  AccelData accel = (AccelData) { .x = 0, .y = 0, .z = 0 };
  accel_service_peek(&accel);

  
  curXAccel=data[0].x;
  curYAccel=data[0].y;
  curZAccel=data[0].z;
  
  //Store in data for bar graph
  
  int xChange= accel_x[index]= curXAccel-accel_x[index];
  int yChange=  accel_y[index]=curYAccel-accel_y[index];
  int zChange = accel_z[index]=curZAccel-accel_z[index];
  index++;
  if (index==GRAPH_WIDTH) index= 0;
  
  accel_x[index]= curXAccel;
  accel_y[index]=curYAccel;
  accel_z[index]=curZAccel;
  

  check_for_gestures(xChange, yChange, zChange, index);
  
}

static void bargraph_layer_update_callback(Layer *me, GContext *context) {
  draw_bar_graph(context, (GPoint){0, 0*GRAPH_HEIGHT/4+offset},(GSize){GRAPH_WIDTH,GRAPH_HEIGHT/3-1}, accel_x);
  draw_bar_graph(context, (GPoint){0, 1*GRAPH_HEIGHT/4+offset},(GSize){GRAPH_WIDTH,GRAPH_HEIGHT/3-1}, accel_y);
  draw_bar_graph(context, (GPoint){0, 2*GRAPH_HEIGHT/4+offset},(GSize){GRAPH_WIDTH,GRAPH_HEIGHT/3-1}, accel_z);
}

static void draw_bar_graph(GContext* ctx, GPoint origin, GSize size, int* list) {
  int i;
  //draw background rect (clear out old stuff)
  GRect backgndRect = { .origin = origin, .size = size };
  graphics_context_set_fill_color(ctx,	GColorWhite);	
  graphics_fill_rect(ctx, backgndRect, 0, GCornerNone );
  //draw bar graph
  for(i=0 ; i < (int) GRAPH_WIDTH ; i++) {
    int xorg=origin.x+i;
    int xdest=xorg; //vertial lines
    int yorg=origin.y + size.h/2;
    int ydest=yorg + list[i]/100;
    //make sure it fits:
    if(ydest < origin.y) ydest = origin.y;
    if(ydest > origin.y + size.h) ydest = origin.y + size.h;
    
    graphics_draw_line(	ctx,
      (GPoint) {xorg, yorg},
      (GPoint) {xdest, ydest} 
    );
  }
}

static void timer_callback(void *data) {
  layer_mark_dirty(bargraph_layer);
  timer = app_timer_register(ACCEL_STEP_MS, timer_callback, NULL);
}
  