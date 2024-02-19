#include "numerify-hebrew-string.hpp"

#include <iostream> // for "std::cerr"


///////////////////////////////////////////////////////////
///        U T F - 8   T O   I S O - 8 8 5 9 - 8        ///
///////////////////////////////////////////////////////////


static void _panic(size_t i, unsigned char c) {
/** TOOD: is this ever called? try gibberish */
  std::cerr 
    << "utf8_to_iso_8859_8 failed: invalid character " << (unsigned)c << " "
    << "at index " << i 
    << std::endl;

  exit(2); // todo have a way to return error as value
}

static std::string utf8_to_iso_8859_8(const std::string &utf8) {
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


///////////////////////////////////////////////////////////
///      I S O - 8 8 5 9 - 8   T O   N U M B E R S      ///
///////////////////////////////////////////////////////////


static uint8_t iso_8859_8_to_number(char letter) {
  switch (letter - '\xE0') {
    case 0 : return   0 ;  //  א
    case 1 : return   1 ;  //  ב
    case 2 : return   2 ;  //  ג
    case 3 : return   3 ;  //  ד
    case 4 : return   4 ;  //  ה
    case 5 : return   5 ;  //  ו
    case 6 : return   6 ;  //  ז
    case 7 : return   7 ;  //  ח
    case 8 : return   8 ;  //  ט
    case 9 : return   9 ;  //  י
    case 10: return   10;  //  ך
    case 11: return   10;  //  כ
    case 12: return   11;  //  ל
    case 13: return   12;  //  ם
    case 14: return   12;  //  מ
    case 15: return   13;  //  ן
    case 16: return   13;  //  נ
    case 17: return   14;  //  ס
    case 18: return   15;  //  ע
    case 19: return   16;  //  ף
    case 20: return   16;  //  פ
    case 21: return   17;  //  ץ
    case 22: return   17;  //  צ
    case 23: return   18;  //  ק
    case 24: return   19;  //  ר
    case 25: return   20;  //  ש
    case 26: return   21;  //  ת
    
    default: 
      std::cerr << "iso_8859_8_to_number failed: non iso-8859-8 hebrew letter" << '\n';
      exit(3);
  }
}


///////////////////////////////////////////////////////////
///           U T F - 8   T O   N U M B E R S           ///
///////////////////////////////////////////////////////////


std::vector<uint8_t> numerify_hebrew_string(const std::string &string_utf8) {
  const std::string string_iso_8859_8 = utf8_to_iso_8859_8(string_utf8);

  std::vector<uint8_t> result;
  
  static const auto is_not_space = [](char c) { return !isspace(c); };

  std::copy_if(string_iso_8859_8.begin(), string_iso_8859_8.end(), std::back_inserter(result), is_not_space);
  std::transform(result.begin(), result.end(), result.begin(), iso_8859_8_to_number);

  return result;
}
