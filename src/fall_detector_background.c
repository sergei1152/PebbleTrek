#include <pebble_worker.h>
#include "accelerometer.h"
#include "config.h"
  
static void init() {
  // Initialize your worker here

}

static void deinit() {
  // Deinitialize your worker here

}

int main(void) {
  init();
  worker_event_loop();
  deinit();
}
