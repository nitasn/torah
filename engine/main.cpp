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