#include "search.hpp"
#include "util-functions.hpp" // for "utf8ToIso8859_8"

#include <iostream>

int main() {
  while (true) {
    std::cout << "search: ";

    std::string patternUTF8;
    std::getline(std::cin, patternUTF8);
    // patternUTF8 = "שמוליקיפוד";
    // patternUTF8 = "יצחק רבין";

    auto result = search(utf8ToIso8859_8(patternUTF8));

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