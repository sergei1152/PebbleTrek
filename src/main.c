#include <pebble.h>
  
static Window *s_main_window;

static void main_window_load(Window *window) {
    app_worker_launch();

}

static void main_window_unload(Window *window) {
  // Destroy Window's child Layers here

}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    
}
static void click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}
static void init() {

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
}
int main(void){
  init();
  app_event_loop();
  deinit();
}