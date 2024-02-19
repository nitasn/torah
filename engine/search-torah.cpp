#include "search-torah.hpp"

#include "numerify-hebrew-string.hpp"
#include "torah_block.h"

std::optional<SearchResult> search_torah(const std::string& pattern_utf8, size_t min_step, size_t max_step) {
  std::vector<uint8_t> simplified_pat = numerify_hebrew_string(pattern_utf8);
  return boyer_moore_els(
    torah_block, torah_block_len,
    simplified_pat.data(), simplified_pat.size(),
    min_step, max_step
  );
}