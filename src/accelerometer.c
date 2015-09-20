#include <pebble.h>
#include "accelerometer.h"
#include "config.h"
#include "custom_math.h"
  
static void fallDetectCallback(){
  fallDetected=true;
  //Show the data
  text_layer_set_text(text_layer, "Fall Detected");
}

static void updateDisplay(){
  snprintf(s_buffer, sizeof(s_buffer), 
     "Max Acc: %d\nMin Acc: %d\nMag Acc: %d", max_acc, min_acc,
      acc_mag[0]
   );
  
  //Show the data
  text_layer_set_text(text_layer, s_buffer);
}

static void printConsole(){
  for(int i=0;i<25;i++){
    APP_LOG(APP_LOG_LEVEL_DEBUG, "%d",acc_mag[i]);
  }
}

void dropDetector(AccelData *data, uint32_t num_samples) {
    int temp_low=10E6, temp_high=0;

  for(int i=0;i<(int)accelorometer_num_samples;i++){
    acc_mag[i]=float_sqrt(data[i].x*data[i].x+data[i].y*data[i].y+data[i].z*data[i].z);
    
    if(acc_mag[i]<temp_low){
      temp_low=acc_mag[i];
      
    }
    if(acc_mag[i]>temp_high){
      temp_high=acc_mag[i];
    }
    if(acc_mag[i]<min_acc){
        min_acc=acc_mag[i];
      }
    if(acc_mag[i]>max_acc){
        max_acc=acc_mag[i];
      }
  }

  if(temp_high>higher_threshold && temp_low<lower_threshold){
    fallDetectCallback();
  }
  else if(fallDetected==false){
    updateDisplay();
  }
  printConsole();
}

