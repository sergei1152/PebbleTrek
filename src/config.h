#pragma once

extern int max_acc; //The minimum acceleration detected
extern int min_acc; //The maximum acceleration detected
extern  const uint32_t accelorometer_num_samples; //Number of samples for the accelerometer to take
extern int acc_mag;
// Long lived buffer
extern char s_buffer[128];
extern TextLayer *text_layer; //For outputting accelorometer values. Remove this after.
