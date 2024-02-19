#pragma once

#include <stdint.h> // for uint8_t
#include <stddef.h> // for size_t
#include <optional>

struct SearchResult {
  size_t index, step;
};

std::optional<SearchResult> boyer_moore_els(
  uint8_t *string, size_t len_string, 
  uint8_t *pattern, size_t len_pattern, 
  size_t min_step, size_t max_step
);