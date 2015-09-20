#include <pebble.h>
#include "pebbletrek_ui.h"

/*************************************
*               MAIN
*************************************/
int main(void){
  init();
  app_event_loop();
  deinit();
}
