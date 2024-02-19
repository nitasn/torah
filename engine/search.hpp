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
 * result consists of 41 (least-significant) bits.
 * bit 0: whether has result (if it's 0 then the rest is invalid)
 * bits 1-20: index in string
 * bits 21-40: step (diloog) size
 */
uint64_t search__packed_result(const char *pattern_utf8);

}