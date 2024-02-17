#include "util-functions.hpp"
#include "iso-8859-8.hpp"

std::vector<uint8_t> removeSpacesAndConvertToNumbers(const std::string &string_iso_8859_8) {
  std::vector<uint8_t> result;

  static const auto is_not_space = [](char c) { return !isspace(c); };

  std::copy_if(string_iso_8859_8.begin(), string_iso_8859_8.end(), std::back_inserter(result), is_not_space);
  std::transform(result.begin(), result.end(), result.begin(), isoHebrew_to_number);

  return result;
}

#include <iostream>

static void _panic(size_t i, unsigned char c) {
  std::cerr 
    << "utf8_to_iso_8859_8 failed: invalid character " << (unsigned)c << " "
    << "at index " << i 
    << std::endl;

  exit(2); // todo have a way to return error as value
}

std::string utf8ToIso8859_8(const std::string &utf8) {
  std::string result;
  result.reserve(utf8.length());

  for (size_t i = 0; i < utf8.size(); ++i) {
    unsigned char c = utf8[i];
    if ((c & 0x80) == 0) { // ASCII character
      result += c;
    }
    else if ((i + 1 < utf8.size()) && ((c & 0xE0) == 0xC0)) {
      // hebrew character (assuming it's a valid 2-byte UTF-8 sequence)
      unsigned char nextChar = utf8[i + 1];
      int unicodePoint = ((c & 0x1F) << 6) | (nextChar & 0x3F);
      int iso8859_8Char = unicodePoint - 1264;
      if (iso8859_8Char >= 0xA0 && iso8859_8Char <= 0xFA) { // valid ISO-8859-8 range for Hebrew
        result += static_cast<char>(iso8859_8Char);
      }
      else _panic(i, c); 
      i++; // skip the next byte, as it's part of the current character
    }
    else _panic(i, c); 
  }

  return result;
}