#include <pebble.h>
#include "accelerometer.h"
#include "custom_math.h"
#include "config.h"
  
static Window *s_main_window;

static void main_window_load(Window *window) {

  // Create Window's child Layers here
  text_layer = text_layer_create(GRect(0, 0, 144, 144));
  text_layer_set_text(text_layer, "Hello, Pebble!");

  // Add as child layer to be included in rendering
  layer_add_child(window_get_root_layer(s_main_window),text_layer_get_layer(text_layer));

  // Compose string of all data for 3 samples



  
}

static void main_window_unload(Window *window) {
  // Destroy Window's child Layers here

}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
     max_acc=0;
     min_acc=10E6;
    fallDetected=false;
}
static void click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}
static void init() {

  //Subscribing to the accelerometer data service
  accel_data_service_subscribe(accelorometer_num_samples, dropDetector);
  accel_service_set_sampling_rate(ACCEL_SAMPLING_100HZ);

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
