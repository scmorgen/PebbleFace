#include <pebble.h>
#include "gestureRecogn.h"
#include <readAccel.h>
#include <graphics.h>
#include <Messaging.h>
/****************Private Variables ******************/
static bool looking_for_jazzhands=0;
static bool looking_for_roof=0;
static bool looking_for_running=0;
static int jazz_hands[2];
static int raise_the_roof[2];
static int running[2];

static int threshold_z_jh= 700;
static int threshold_x_rr= 500;
static int threshold_5= 500;
static int threshold_y=400;
static int jazz_count=0;
static int count=0;
static int gestures_to_send[4];

/****************Private Declarations*****************/
static void checkJazzHands(int zChange);
static void resetJazzHands();
static void resetRaiseTheRoof();
static void checkRaiseTheRoof(int xChange);
static void resetRunning() ;
static void checkRunning(int yChange);
static void sendThisGesture(bool updateText, char * output, int gestureKey);

/*****************Public Functions *********************/ 
//Gestures
  //Jazz hands consists of multiple high peaks of alternating negative and positive on Z Axis
  //Poke consists of a single high z peak
  //Raise the roof consists of multiple medium peaks of alternating negative and positive on X axis 
  //Running: medium peaks of alternating negative and postive on y axis
void check_for_gestures(int xChange, int yChange, int zChange, int index) {
  if ((zChange>=threshold_5)) {
      count=15;
      gestures_to_send[1]= 1;
  }
  
  //looking for jazz hands
  if (abs(zChange)>=threshold_z_jh) {
    jazz_count=12;
    count=15;
    checkJazzHands(zChange);    
  }
  
  //looking for raise the roof
  if (abs(xChange)>=threshold_x_rr) {
    count=15;
    checkRaiseTheRoof(xChange);
  }
  
  if (abs(yChange)>=threshold_y) {
    count=15;
    checkRunning(yChange);
  }
  
  
  
//  High Five is one high on Z axis
//  Raise the roof is positiveand negative on x axis
  
  if (count<=0){
    text_layer_set_text(gesture_layer, "Last Gesture: None");
    resetRunning();
    resetRaiseTheRoof();
  }
  
  if (jazz_count<=0) {
    resetJazzHands();
  }
  count--;
  jazz_count--;
  
  if (index %6 ==0) {
    bool updateText= (index %12 ==0);
    if (gestures_to_send[3]) sendThisGesture(updateText, "Last Gesture: Running!", RUNNING);
    else if (gestures_to_send[2]) sendThisGesture(updateText, "Last Gesture: RaiseTheRoof", RAISE_THE_ROOF );
    else if (gestures_to_send[0]) sendThisGesture(updateText, "Last Gesture: JazzHands!", JAZZ_HANDS);
    else if (gestures_to_send[1]) sendThisGesture(updateText, "Last Gesture: Poke!", POKE);
    gestures_to_send[3]= gestures_to_send[2]= gestures_to_send[0]= gestures_to_send[1]= 0;
  }
}

/********************Private Functions ****************/
//handles variables checking for jazzhands and sends message if it recognizes them
static void checkJazzHands(int zChange) {
    if (!looking_for_jazzhands) looking_for_jazzhands=1;
  
    if (zChange>0) jazz_hands[0]++; //increment positive change count
    else jazz_hands[1]++; //increment negative change count
    
    if (jazz_hands[0]>=3 && jazz_hands[1]>=3) {
      gestures_to_send[0]= 1;
      resetJazzHands();
    }
}

//reset jazz hands count
void resetJazzHands() {
  jazz_hands[0]= jazz_hands[1]=0;
  looking_for_jazzhands=0;
}

//resets Raise the Roof Count
static void resetRaiseTheRoof() {
  raise_the_roof[0]= raise_the_roof[1]=0;
  looking_for_roof=0;
}
//handles variables checking for jazzhands and sends message if it recognizes them
static void checkRaiseTheRoof(int xChange) {
    int iterations=2;
    if (!looking_for_roof) looking_for_roof=1;
    
    if (xChange>0) raise_the_roof[0]++; //increment positive change count
    else raise_the_roof[1]++; //increment negative change count
    
    if (raise_the_roof[0]>=iterations && raise_the_roof[1]>=iterations) {
      gestures_to_send[2]= 1;
      resetRaiseTheRoof();
    }
}

//resets Raise the Roof Count
static void resetRunning() {
  running[0]= running[1]=0;
  looking_for_running=0;
}
//handles variables checking for jazzhands and sends message if it recognizes them
static void checkRunning(int yChange) {
    int iterations=4;
    if (!looking_for_running) looking_for_running=1;
    
    if (yChange>0) running[0]++; //increment positive change count
    else running[1]++; //increment negative change count
    
    if (running[0]>=iterations && running[1]>=iterations) {
      gestures_to_send[3]= 1;
      resetRunning();
    }
}

//Send A gesture
static void sendThisGesture(bool updateText, char * output, int gestureKey) {
  if (updateText) {
     text_layer_set_text(gesture_layer, output);
  }
   sendGesture(gestureKey);
}
