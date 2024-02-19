#pragma once

#include <string>
#include <optional>

struct SearchResult {
  size_t index, step;
};

// for internal ergonomic use
std::optional<SearchResult> search(const std::string& pattern_utf8);

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
EMSCRIPTEN_KEEPALIVE
#endif

extern "C" {

/** 
 * for wasm to export. 
 * 
 * if there's a match:
 *    the return value has 41 (least-significant) bits
 *    bit 0       set to one
 *    bits 1-20   index in string
 *    bits 21-40  step (diloog) size
 * 
 * else:
 *    he funcion returns 0
 */
uint64_t search__packed_result(const char *pattern_utf8);

}