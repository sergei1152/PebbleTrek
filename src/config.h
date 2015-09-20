#pragma once
#define TAP_NOT_DATA true
#include <stdbool.h>
  
extern int max_acc; //The minimum acceleration detected
extern int min_acc; //The maximum acceleration detected
extern  const int accelorometer_num_samples; //Number of samples for the accelerometer to take
extern int acc_mag[25];
// Long lived buffer
extern char s_buffer[128];
extern int lower_threshold;
extern int higher_threshold;
extern bool fallDetected;
