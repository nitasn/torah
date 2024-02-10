#include "boyer-moore-els.hpp"

#include "read-file.hpp" // for "readBinaryFile"
#include "util-functions.hpp" // "removeSpacesAndConvertToNumbers"

#include <iostream>

// #include <iconv.h> // maybe use this?

int main() {
  std::vector<uint8_t> torahBlock = readBinaryFile("torah.block");

  // torahBlock = std::vector<uint8_t>(torahBlock.begin(), torahBlock.begin() + 50);

  std::string input;
  std::getline(std::cin, input); // read entire line

  input = removeSpacesAndConvertToNumbers(input);

  size_t step = 0;

  uint8_t *result = boyer_moore_els(
    torahBlock.data(), torahBlock.size(),
    (uint8_t *)input.data(), input.size(),
    &step
  );

  if (!result) {
    std::cerr << "no result" << '\n';
    exit(1);
  }

  std::cout << "found" << '\n';
  std::cout << "at index " << (size_t)(result) - (size_t)(torahBlock.data()) << '\n';
  std::cout << "at step " << step << '\n';
}