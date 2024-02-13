#include "util-functions.hpp"
#include "iso-8859-8.hpp"

#include <string>

std::string removeSpacesAndConvertToNumbers(const std::string& input) {
  std::string result;

  static const auto is_not_space = [](char c) { return !isspace(c); };
  
  std::copy_if(input.begin(), input.end(), std::back_inserter(result), is_not_space);
  std::transform(result.begin(), result.end(), result.begin(), isoHebrew_to_number);

  return result;
}

#include <iconv.h>
#include <iostream> // for "std::cerr"
#include <vector>

struct iconv_singleton_t{
  iconv_t conv = iconv_open("ISO-8859-8", "UTF-8");
  ~iconv_singleton_t() { iconv_close(conv); }
} iconv_singleton;

std::string utf8_to_iso88598(const std::string& utf8String) {
  if (iconv_singleton.conv == (iconv_t)-1) {
    std::cerr << "failed to open iconv" << std::endl;
    exit(2);
  }

  size_t inBytesLeft = utf8String.size();
  size_t outBytesLeft = inBytesLeft;
  std::vector<char> outBuffer(outBytesLeft);
  char* inPtr = const_cast<char*>(utf8String.data());
  char* outPtr = outBuffer.data();

  // reset conversion descriptor
  iconv(iconv_singleton.conv, nullptr, nullptr, nullptr, nullptr);

  size_t result = iconv(iconv_singleton.conv, &inPtr, &inBytesLeft, &outPtr, &outBytesLeft);
  if (result == (size_t)-1) {
    perror("converting utf-8 to iso-8859-9 with iconv failed");
    exit(2);
  }

  size_t writtenBytes = outBuffer.size() - outBytesLeft;
  return std::string(outBuffer.data(), writtenBytes);
}
