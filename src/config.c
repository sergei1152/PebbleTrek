#include <pebble.h>
#include "config.h"

int max_acc=0; //The minimum acceleration detected
int min_acc=10E6; //The maximum acceleration detected
const uint32_t accelorometer_num_samples=25; //Number of samples for the accelerometer to take

int acc_mag[25]; //make sure you change this too if you change samples

// Long lived buffer
char s_buffer[128];
TextLayer *text_layer; //For outputting accelorometer values. Remove this after.
int lower_threshold=250;
int higher_threshold=4000;
bool fallDetected=false;
