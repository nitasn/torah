#include "search.hpp"

#include "numerify-hebrew-string.hpp"
#include "boyer-moore-els.hpp"
#include "torah_block.h"

std::optional<SearchResult> search(const std::string& pattern_utf8) {
  std::vector<uint8_t> simplified_pat = numerify_hebrew_string(pattern_utf8);

  size_t step = 0;

  uint8_t *pMatch = boyer_moore_els(
    torah_block, torah_block_len,
    simplified_pat.data(), simplified_pat.size(),
    &step
  );

  if (!pMatch) return std::nullopt;

  size_t index = (ptrdiff_t) pMatch - (ptrdiff_t) torah_block;

  return {{
    .index = index,
    .step = step
  }};
}