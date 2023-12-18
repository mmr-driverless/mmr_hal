#ifndef LERP_H
#define LERP_H

#include <util.h>

static inline float lerp2(const float x, const MmrPointF start, const MmrPointF end) {
  const float M = end.y - start.y;
  const float X = (x - start.x) / (end.x - start.x);
  const float Q = start.y;
  
  return M * X + Q;
}

static inline float lerp3(const float x, const MmrPointF start, const MmrPointF p1, const MmrPointF end) {
  return x < p1.x? lerp2(x, start, p1) : lerp2(x, p1, end);
}

static inline float lerp4(const float x, const MmrPointF start, const MmrPointF p1, const MmrPointF p2, const MmrPointF end) {
  return x < p2.x? lerp3(x, start, p1, p2) : lerp2(x, p2, end);
}

#endif // !MMR_LERP_H
