#include "inc/interval.h"

#include <stdint.h>
#include <stdbool.h>


float clamp(float val, float inf, float sup) {
  float v = val < inf ? inf : val;
  return v > sup ? sup : v;
}


float normalize(float val, float min, float max) {
  return (val - min) / (max - min);
}


float linear_rescale(float val, float min, float max, float inf, float sup) {
  float scaleK = (sup - inf) / (max - min);
  return scaleK * (val - min) + inf;
}