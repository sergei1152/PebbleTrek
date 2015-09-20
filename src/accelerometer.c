#include <pebble.h>
#include "accelerometer.h"
#include "config.h"
#include "custom_math.h"
  
static void updateDisplay(){
  snprintf(s_buffer, sizeof(s_buffer), 
     "Max Acc: %d\nMin Acc: %d\nMag Acc: %d", max_acc, min_acc,
     acc_mag
   );
  
  //Show the data
  text_layer_set_text(text_layer, s_buffer);
}

static void printConsole(){
     APP_LOG(APP_LOG_LEVEL_DEBUG, "Max Acc: %d\nMin Acc: %d\nMag Acc: %d", max_acc, min_acc,
     acc_mag
   );
}

void dropDetector(AccelData *data, uint32_t num_samples) {
  acc_mag=float_sqrt(data[0].x*data[0].x+data[0].y*data[0].y+data[0].z*data[0].z);
  if(acc_mag>max_acc){
    max_acc=acc_mag;
  }
   else if(acc_mag<min_acc){
    min_acc=acc_mag;
   }
  
  updateDisplay();
  printConsole();
}

