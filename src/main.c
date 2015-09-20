#include <pebble.h>
#include "pebbletrek_ui.h"
#include "pebbletrek_message.h"

/*************************************
*               MAIN
*************************************/
int main(void){
  init();
  register_messages();
  app_event_loop();
  deinit();
}
