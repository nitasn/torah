#include "search-torah.hpp"

#include <iostream>

int main() {
  while (true) {
    std::cout << "search: ";

    std::string patternUTF8;
    std::getline(std::cin, patternUTF8);
    if (patternUTF8 == "quit") break;

    auto result = search_torah(patternUTF8);

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