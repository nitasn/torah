#include "search.hpp"

#include "boyer-moore-els.hpp"
#include "util-functions.hpp" // "removeSpacesAndConvertToNumbers"
#include "torah_block.h"

std::optional<SearchResult> search(const std::string& pattern_utf8) {
  std::string pattern_iso_8859_8 = utf8ToIso8859_8(pattern_utf8);
  std::vector<uint8_t> simplified_pat = removeSpacesAndConvertToNumbers(pattern_iso_8859_8);

  size_t step = 0;

  uint8_t *pMatch = boyer_moore_els(
    torah_block, torah_block_len,
    (uint8_t *)simplified_pat.data(), simplified_pat.size(),
    &step
  );

  if (!pMatch) return std::nullopt;

  size_t index = (ptrdiff_t) pMatch - (ptrdiff_t) torah_block;

  return {{
    .index = index,
    .step = step
  }};
}