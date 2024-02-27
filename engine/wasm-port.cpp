#include "search-torah.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
EMSCRIPTEN_KEEPALIVE
#endif

/** 
 * for wasm to export. 
 */
extern "C" {
  int c_style__search(const char *pattern_utf8, uint32_t min_step, uint32_t max_step, uint32_t *index, uint32_t *step);
}

/// impl

int c_style__search(const char *pattern_utf8, uint32_t min_step, uint32_t max_step, uint32_t *index, uint32_t *step) {
  auto result = search_torah(pattern_utf8, min_step, max_step);

  if (!result) {
    return 0;
  }
  
  *index = result->index;
  *step = result->step;
  return 1;
}