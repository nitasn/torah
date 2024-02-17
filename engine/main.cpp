#include "boyer-moore-els.hpp"
#include "util-functions.hpp" // "removeSpacesAndConvertToNumbers"
#include "torah_block.h"

#include <iostream>

int main() {
  while (true) {
    std::cout << "search: ";

    std::string original_input;
    // std::getline(std::cin, original_input);
    original_input = "שמוליקיפוד";

    std::string simplified_input = removeSpacesAndConvertToNumbers(utf8_to_iso88598(original_input));

    if (simplified_input.size() == 0) break;

    size_t step = 0;
    uint8_t *result = boyer_moore_els(
      torah_block, torah_block_len,
      (uint8_t *)simplified_input.data(), simplified_input.size(),
      &step
    );

    if (result) {
      auto index = (size_t)(result) - (size_t)(torah_block);
      std::cout << "found " << original_input << '\n';
      std::cout << "at index " << index << '\n';
      std::cout << "at step " << step << '\n';
    }
    else {
      std::cout << "could not find " << original_input << '\n';
    }

    return 0;
  }
}
