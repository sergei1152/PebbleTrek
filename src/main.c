#include <pebble.h>
#include <math.h>
#include <stdlib.h>
#include "custom_math.h"

  

static Window *s_main_window;
TextLayer *text_layer; //For outputting accelorometer values. Remove this after.

//accelorometer settings
int max_acc=0;
int min_acc=10E6;
const uint32_t accelorometer_num_samples=1; //Number of samples for the accelerometer to take

static void main_window_load(Window *window) {
  // Create Window's child Layers here
  text_layer = text_layer_create(GRect(0, 0, 144, 144));
  text_layer_set_text(text_layer, "Hello, Pebble!");

  // Add as child layer to be included in rendering
  layer_add_child(window_get_root_layer(s_main_window),text_layer_get_layer(text_layer));
}

static void dropDetector(AccelData *data, uint32_t num_samples) {
  // Long lived buffer
  static char s_buffer[128];
  
  int acc_mag=float_sqrt(data[0].x*data[0].x+data[0].y*data[0].y+data[0].z*data[0].z);
  if(acc_mag>max_acc){
    max_acc=acc_mag;
  }
   else if(acc_mag<min_acc){
    min_acc=acc_mag;
  }
 
  // Compose string of all data for 3 samples
  snprintf(s_buffer, sizeof(s_buffer), 
    "N X,Y,Z\n0 %d,%d,%d\n Max Acc: %d\nMin Acc: %d\nMag Acc: %d", 
    data[0].x, data[0].y, data[0].z,
           max_acc, min_acc,
    acc_mag
  );

  APP_LOG(APP_LOG_LEVEL_DEBUG, "N X,Y,Z\n0 %d,%d,%d\n Max Acc: %d\nMin Acc: %d\nMag Acc: %d",  
   data[0].x, data[0].y, data[0].z,
          max_acc, min_acc,
    acc_mag
  );

  //Show the data
  text_layer_set_text(text_layer, s_buffer);
}

static void main_window_unload(Window *window) {
  // Destroy Window's child Layers here

}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
     max_acc=0;
     min_acc=10E6;
}
static void click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}
static void init() {

  //Subscribing to the accelerometer data service
  accel_data_service_subscribe(accelorometer_num_samples, dropDetector);
  accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ);

  // Create main Window
  s_main_window = window_create();

  //event handlers for windows load and unload
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  
  //detecting button click
  window_set_click_config_provider(s_main_window, click_config_provider);


  window_stack_push(s_main_window, true);
}

static void deinit() {
  // Destroy main Window
  window_destroy(s_main_window);
  // Destroy the TextLayer
  text_layer_destroy(text_layer);
}
int main(void){
  init();
  app_event_loop();
  deinit();
}
