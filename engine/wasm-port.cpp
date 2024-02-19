#include "search.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
EMSCRIPTEN_KEEPALIVE
#endif

/** 
 * for wasm to export. 
 */
extern "C" {
  void c_style__search(const char *pattern_utf8, int32_t *index, int32_t *step);
}

/// impl

void c_style__search(const char *pattern_utf8, int32_t *index, int32_t *step) {
  auto result = search(pattern_utf8);
  
  if (result) {
    *index = result->index;
    *step = result->step;
  }
  else {
    *index = -1;
    *step = 0;
  }
}