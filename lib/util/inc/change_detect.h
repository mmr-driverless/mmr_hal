#define IF_CHANGED_DO(type, x, action) { \
  static type __old##x; \
  if (x != __old##x) { \
    __old##x = x; \
    action \
  } \
}
