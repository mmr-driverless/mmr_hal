#ifndef APP_LIB_UTIL_INC_INTERVAL_H_
#define APP_LIB_UTIL_INC_INTERVAL_H_

float clamp(float val, float inf, float sup);

/**
 * Normalize a value that lives in [min, max] interval to [0, 1]
 * @param val must stay in [min, max] for working properly
 * @return a real number in [0, 1]
*/
float normalize(float val, float min, float max);

/**
 * Scale linearly a value in [min, max] to [inf, sup]
*/
float linear_rescale(float val, float min, float max, float inf, float sup);

#endif  // !APP_LIB_UTIL_INC_INTERVAL_H_