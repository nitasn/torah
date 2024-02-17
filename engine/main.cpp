#include "boyer-moore-els.hpp"
#include "util-functions.hpp" // "removeSpacesAndConvertToNumbers"
#include "torah_block.h"


#include <optional>

struct SearchResult {
  size_t index, step;
};

std::optional<SearchResult> search(const std::string& patternUTF8) {
  std::string simplified_input = removeSpacesAndConvertToNumbers(utf8ToIso8859_8(patternUTF8));

  size_t step = 0;

  uint8_t *result = boyer_moore_els(
    torah_block, torah_block_len,
    (uint8_t *)simplified_input.data(), simplified_input.size(),
    &step
  );

  if (!result) return std::nullopt;

  size_t index = (ptrdiff_t) result - (ptrdiff_t) torah_block;

  return {{
    .index = index,
    .step = step,
  }};
}


#include <iostream>

int main() {
  while (true) {
    std::cout << "search: ";

    std::string patternUTF8;
    std::getline(std::cin, patternUTF8);
    // patternUTF8 = "שמוליקיפוד";
    // patternUTF8 = "יצחק רבין";

    auto result = search(patternUTF8);

    if (result) {
      std::cout << "found " << patternUTF8 << '\n';
      std::cout << "at index " << result->index << '\n';
      std::cout << "at step " << result->step << '\n';
    }
    else {
      std::cout << "could not find " << patternUTF8 << '\n';
    }
  }
}