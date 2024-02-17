#pragma once

#include <stdint.h> // for uint8_t
#include <stddef.h> // for size_t
#include <vector>

// struct SearchResult {

// };

uint8_t *boyer_moore_els(
  // std::vector<uint8_t>
  uint8_t *string, size_t stringlen, 
  uint8_t *pat, size_t patlen, 
  size_t *ptr_step // a.k.a the "diloog"
);