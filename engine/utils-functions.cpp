#include "util-functions.hpp"
#include "iso-8859-8.hpp"

#include <string>

std::string removeSpacesAndConvertToNumbers(const std::string &input) {
  std::string result;

  static const auto is_not_space = [](char c) { return !isspace(c); };

  std::copy_if(input.begin(), input.end(), std::back_inserter(result), is_not_space);
  std::transform(result.begin(), result.end(), result.begin(), isoHebrew_to_number);

  return result;
}


#include <iostream>

std::string utf8ToIso8859_8(const std::string &utf8) {
  std::string result;

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
      else {
        std::cerr << "Found invalid Hebrew character in UTF-8 string" << std::endl;
        exit(2);
      }
      i++; // skip the next byte, as it's part of the current character
    }
    else {
      std::cerr << "Unsupported character encountered" << std::endl;
      exit(2);
    }
  }

  return result;
}