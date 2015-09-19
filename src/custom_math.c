#include <pebble.h>
#include "custom_math.h"

  //TODO: Convert this to a integer sqrt for better efficiency
float float_sqrt(const float num) {
  const uint MAX_STEPS = 40;
  const float MAX_ERROR = 0.001;
  
  float answer = num;
  float ans_sqr = answer * answer;
  uint step = 0;
  while((ans_sqr - num > MAX_ERROR) && (step++ < MAX_STEPS)) {
    answer = (answer + (num / answer)) / 2;
    ans_sqr = answer * answer;
  }
  return answer;
}
