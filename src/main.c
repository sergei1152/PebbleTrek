#include <pebble.h>
#include "pebbletrek_ui.h"
#include "pebbletrek_message.h"
  
/*************************************
*               MAIN
*************************************/
int main(void){
  int init_state = init();
  if (init_state == 0) {
    deinit();
  }
  register_messages();
  app_event_loop();
  deinit();
}