#ifndef EDGE_DETECT_H
#define EDGE_DETECT_H

#include <stdbool.h>

typedef enum MmrEdge {
  MMR_EDGE_NONE,
  MMR_EDGE_RISING,
  MMR_EDGE_FALLING
} MmrEdge;

static inline MmrEdge MMR_EDGE_Detect(bool* oldValue, bool newValue) {
  if (*oldValue != newValue) {
    *oldValue = newValue;
    return newValue? MMR_EDGE_RISING : MMR_EDGE_FALLING;
  }
  return MMR_EDGE_NONE;
}

#define ON_EDGE_DO(edge, x, action) { \
  static bool __old##x; \
  if (MMR_EDGE_Detect(&__old##x, x) == edge) { \
    action \
  } \
}
#define ON_NO_EDGE_DO(x, action) ON_EDGE_DO(MMR_EDGE_NONE, x, action)
#define ON_RISING_EDGE_DO(x, action) ON_EDGE_DO(MMR_EDGE_RISING, x, action)
#define ON_FALLING_EDGE_DO(x, action) ON_EDGE_DO(MMR_EDGE_RISING, x, action)

#endif // !EDGE_DETECT_H