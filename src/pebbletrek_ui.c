#include <pebble.h>
#include "pebbletrek_ui.h"
#include "pebbletrek_message.h"
    
enum appState {
  app_state_interrupt = 0,
  app_state_countdown = 1,
  app_state_sending = 2
};

static int app_state_now;
static int s_uptime = 0;
static int countdown_time_total;
static int countdown_time_left;
static bool stl_sending_c_glow = false;
  
static Window *s_interrupt_window;
static TextLayer *stl_fall;
  
static Window *s_countdown_window;
static TextLayer *stl_sending_in;
static TextLayer *stl_time;
static TextLayer *stl_cancel;

static Window *s_sending_window;
static TextLayer *stl_sending_to;
char scp_name[] = "default";
static TextLayer *stl_phone;



void get_contact_name(char *name){
  if (persist_exists(NAME)){
    persist_read_string(NAME, name, sizeof(name));
  }
  else {
    name = "[Please set up app]";
  }
  APP_LOG(APP_LOG_LEVEL_DEBUG, "name = %s", name);
  text_layer_set_text(stl_phone, name);
}

void get_contact_number(char *number){
  if (persist_exists(NUMBER)){
    persist_read_string(NUMBER, number, sizeof(number));
  }
  else {
    number = "[Please set up app]";
  }
}

int get_countdown_duration(){
  if (persist_exists(COUNTDOWN_DURATION)){
    return (int)(persist_read_int(COUNTDOWN_DURATION));
  }
  else {
    return 15;
  }
}

void set_countdown_time_total(){
  countdown_time_total = get_countdown_duration();
}

/*************************************
*          INTERRUPT WINDOW
*************************************/
static void interrupt_window_load(Window *window) {
  stl_fall = text_layer_create(GRect(0, 60, 144, 60));
  text_layer_set_background_color(stl_fall, GColorClear);
  text_layer_set_text_color(stl_fall, GColorWhite);
  text_layer_set_font(stl_fall, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(stl_fall, GTextAlignmentCenter);
  text_layer_set_text(stl_fall, "FALL DETECTED");
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(stl_fall));
}

static void interrupt_window_unload(Window *window) {
  text_layer_destroy(stl_fall);
}

/*************************************
*          COUNTDOWN WINDOW
*************************************/
static void countdown_window_load(Window *window) {
  // Create Window's child Layers here 
  
  stl_sending_in = text_layer_create(GRect(5, 0, 134, 100));
  text_layer_set_background_color(stl_sending_in, GColorClear);
  text_layer_set_text_color(stl_sending_in, GColorWhite);
  text_layer_set_font(stl_sending_in, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(stl_sending_in, GTextAlignmentLeft);
  text_layer_set_text(stl_sending_in, "Sending SMS to emergency contact in");
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(stl_sending_in));
  
  stl_time = text_layer_create(GRect(5, 35, 130, 58));
  text_layer_set_background_color(stl_time, GColorClear);
  text_layer_set_text_color(stl_time, GColorWhite);
  text_layer_set_font(stl_time, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  text_layer_set_text_alignment(stl_time, GTextAlignmentRight);
  text_layer_set_text(stl_time, "15 sec");
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(stl_time));
  
  stl_cancel = text_layer_create(GRect(5, 78, 134, 90));
  text_layer_set_background_color(stl_cancel, GColorClear);
  text_layer_set_text_color(stl_cancel, GColorWhite);
  text_layer_set_font(stl_cancel, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(stl_cancel, GTextAlignmentLeft);
  text_layer_set_text(stl_cancel, "Press BACK button to CANCEL emergency.");
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(stl_cancel));
  
}

static void countdown_window_unload(Window *window) {
  // Destroy Window's child Layers here
  text_layer_destroy(stl_sending_in);
  text_layer_destroy(stl_time);
  text_layer_destroy(stl_cancel);
}

/*************************************
*          SENDING WINDOW
*************************************/
static void sending_window_load(Window *window) {
  // Create Window's child Layers here 
  
  stl_sending_to = text_layer_create(GRect(5, 0, 134, 100));
  text_layer_set_background_color(stl_sending_to, GColorClear);
  text_layer_set_text_color(stl_sending_to, GColorBlack);
  text_layer_set_font(stl_sending_to, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(stl_sending_to, GTextAlignmentLeft);
  text_layer_set_text(stl_sending_to, "Emergency mode activated.\n\nSending SMS...");
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(stl_sending_to));
  
  stl_phone = text_layer_create(GRect(5, 95, 134, 60));
  text_layer_set_background_color(stl_phone, GColorClear);
  text_layer_set_text_color(stl_phone, GColorBlack);
  text_layer_set_font(stl_phone, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(stl_phone, GTextAlignmentLeft);
  
  get_contact_name(scp_name);
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "scp_name = %s", scp_name);
  
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(stl_phone));
}

static void sending_window_unload(Window *window) {
  // Destroy Window's child Layers here
  text_layer_destroy(stl_sending_to);
  text_layer_destroy(stl_phone);
}

/*************************************
*            TICK HANDLER
*************************************/
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "entered tick_handler");
  if ((s_uptime == 3) && (app_state_now == app_state_interrupt)){
    APP_LOG(APP_LOG_LEVEL_DEBUG, "changing app state to app_state_countdown");
    app_state_now = app_state_countdown;
    APP_LOG(APP_LOG_LEVEL_DEBUG, "pushing s_countdown_window to window stack");
    window_stack_push(s_countdown_window, true);
    countdown_time_left = countdown_time_total;
  }
  else if ((s_uptime > 3) && (s_uptime <= 3 + countdown_time_total)){
    APP_LOG(APP_LOG_LEVEL_DEBUG, "decrementing countdown_time_left to %d", countdown_time_left);
    countdown_time_left--;
    static char buffer[] = "15 sec";
    snprintf(buffer, 7, "%d sec", countdown_time_left);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "buffer = '%s'", buffer);
    text_layer_set_text(stl_time, buffer);
    vibes_long_pulse();
  }
  else if ((s_uptime > 3 + countdown_time_total) && (app_state_now == app_state_countdown)){
    APP_LOG(APP_LOG_LEVEL_DEBUG, "changing app state to app_state_sending");
    app_state_now = app_state_sending;
    APP_LOG(APP_LOG_LEVEL_DEBUG, "pushing s_sending_window to window stack");
    window_stack_push(s_sending_window, true);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "double pulse vibration");
    vibes_double_pulse();
    // Send notify command to pebble js app
    APP_LOG(APP_LOG_LEVEL_DEBUG, "send int notify 1");
    send_int(NOTIFY, 1);
  }
  else if ((s_uptime > 3 + countdown_time_total) && (app_state_now == app_state_sending)){
    stl_sending_c_glow = !stl_sending_c_glow;
    if (stl_sending_c_glow){
      text_layer_set_text_color(stl_sending_to, GColorRed);
      text_layer_set_text_color(stl_phone, GColorRed);
    }
    else{
      text_layer_set_text_color(stl_sending_to, GColorBlack);
      text_layer_set_text_color(stl_phone, GColorBlack);
    }
  }
  
  s_uptime++;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "s_uptime = %d\napp_state_now = %d", s_uptime, app_state_now);
}


/*************************************
*            CLICK HANDLER
*************************************/
static void back_click_handler(ClickRecognizerRef recognizer, void *context){
  window_stack_pop(true);
  window_stack_pop(true);
  window_stack_pop(true);
  
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, NULL);
  window_single_click_subscribe(BUTTON_ID_SELECT, NULL);
  window_single_click_subscribe(BUTTON_ID_DOWN, NULL);
  window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);
}

/*************************************
*          INIT AND DEINIT
*************************************/
int init() {
  
  if(app_worker_is_running()){
    app_state_now = app_state_interrupt;
    set_countdown_time_total();
    
    // Create interrupt window
    s_interrupt_window = window_create();
    window_set_window_handlers(s_interrupt_window, (WindowHandlers) {
      .load = interrupt_window_load,
      .unload = interrupt_window_unload,
    });
    window_set_click_config_provider(s_interrupt_window, click_config_provider);
    window_set_background_color(s_interrupt_window, GColorRed);
    
    // Create countdown window
    s_countdown_window = window_create();
    window_set_window_handlers(s_countdown_window, (WindowHandlers) {
      .load = countdown_window_load,
      .unload = countdown_window_unload,
    });
    window_set_click_config_provider(s_countdown_window, click_config_provider);
    window_set_background_color(s_countdown_window, GColorRed);
    
    // Create sending window
    s_sending_window = window_create();
    window_set_window_handlers(s_sending_window, (WindowHandlers) {
      .load = sending_window_load,
      .unload = sending_window_unload,
    });
    window_set_click_config_provider(s_sending_window, click_config_provider);
    window_set_background_color(s_sending_window, GColorYellow);
    
    
    // Push interrupt window
    window_stack_push(s_interrupt_window, true);
    tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
    vibes_long_pulse();
    return 1;
  }
  else{
    app_worker_launch();
    return 0;
  }
}

int deinit() {
  // Destroy main Window
  window_destroy(s_interrupt_window);
  window_destroy(s_countdown_window);
  window_destroy(s_sending_window);
  return 0;
}
