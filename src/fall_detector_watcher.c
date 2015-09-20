#include "config.h"
#include <pebble_worker.h>
#include "custom_math.h"
  
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
    worker_launch_app();
    app_worker_send_message(0, NULL);
  }
  else if(fallDetected==false){
  }
  APP_LOG(APP_LOG_LEVEL_DEBUG, "running");
}

static void init() {
  // Initialize your worker here
  //Subscribing to the accelerometer data service
  accel_data_service_subscribe((int32_t)accelorometer_num_samples, dropDetector);
  accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ);
}

static void deinit() {
  // Deinitialize your worker here

}


int main(void) {
  init();
  worker_event_loop();
  deinit();
}