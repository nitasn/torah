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
uint64_t search__packed_result(const char *pattern_utf8) {
  std::optional<SearchResult> result = search(pattern_utf8);
  if (!result) return 0;
  const uint64_t twenty_bits = (1 << 20) - 1;
  const uint64_t step = result->step & twenty_bits;
  const uint64_t index = result->index & twenty_bits;
  return (step << 21) | (index << 1) | 0b1;
}