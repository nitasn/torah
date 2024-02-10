#include "boyer-moore-els.hpp"

#include "read-file.hpp" // for "readBinaryFile"
#include "util-functions.hpp" // "removeSpacesAndConvertToNumbers"

#include <iostream>

// #include <iconv.h> // maybe use this?

int main() {
  std::vector<uint8_t> torahBlock = readBinaryFile("torah.block");

  torahBlock = std::vector<uint8_t>(torahBlock.begin(), torahBlock.begin() + 50);

  std::string input;
  std::cin >> input;

  input = removeSpacesAndConvertToNumbers(input);

  uint8_t *result = boyer_moore_els(
    torahBlock.data(), torahBlock.size(),
    (uint8_t *)input.data(), input.size(),
    2
  );

  if (!result) {
    std::cerr << "no result" << '\n';
    exit(1);
  }

  std::cout << "found it at index " << (size_t)(result) - (size_t)(torahBlock.data()) << '\n';
}

/*

49 48 47 46 45 44 43 42 41 40 39 38 37 36 35 34 33 32 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9  8  7  6  5  4  3  2  1  0
ב  ר  א  ש  י  ת  ב  ר  א  א  ל  ה  י  ם  א  ת  ה  ש  מ  י  ם  ו  א  ת  ה  א  ר  ץ  ו  ה  א  ר  ץ  ה  י  ת  ה  ת  ה  ו  ו  ב  ה  ו  ו  ח  ש  ך  ע  ל

*/