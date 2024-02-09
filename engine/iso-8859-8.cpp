#include "iso-8859-8.hpp"

#include <stdexcept>


/**
 * avoiding having \0 chars for now.
 * todo: later when it works, do try to decrease alphabet length.
 */

char isoHebrew_to_number(char letter) {
  switch (letter - '\xE0') {
    case 0 : return   1 ;  //  א
    case 1 : return   2 ;  //  ב
    case 2 : return   3 ;  //  ג
    case 3 : return   4 ;  //  ד
    case 4 : return   5 ;  //  ה
    case 5 : return   6 ;  //  ו
    case 6 : return   7 ;  //  ז
    case 7 : return   8 ;  //  ח
    case 8 : return   9 ;  //  ט
    case 9 : return   10;  //  י
    case 10: return   11;  //  ך
    case 11: return   11;  //  כ
    case 12: return   12;  //  ל
    case 13: return   13;  //  ם
    case 14: return   13;  //  מ
    case 15: return   14;  //  ן
    case 16: return   14;  //  נ
    case 17: return   15;  //  ס
    case 18: return   16;  //  ע
    case 19: return   17;  //  ף
    case 20: return   17;  //  פ
    case 21: return   18;  //  ץ
    case 22: return   18;  //  צ
    case 23: return   19;  //  ק
    case 24: return   20;  //  ר
    case 25: return   21;  //  ש
    case 26: return   22;  //  ת
    default: throw std::invalid_argument("non iso-8859-8 hebrew letter");
  }
}

char number_to_isoHebrew(char number) {
  /**
   * avoiding having \0 chars for now.
   * todo: later when it works, do try to decrease alphabet length.
   */
  switch (number) {
    case 1 : return  '\xE0' + 0 ;  //  א
    case 2 : return  '\xE0' + 1 ;  //  ב
    case 3 : return  '\xE0' + 2 ;  //  ג
    case 4 : return  '\xE0' + 3 ;  //  ד
    case 5 : return  '\xE0' + 4 ;  //  ה
    case 6 : return  '\xE0' + 5 ;  //  ו
    case 7 : return  '\xE0' + 6 ;  //  ז
    case 8 : return  '\xE0' + 7 ;  //  ח
    case 9 : return  '\xE0' + 8 ;  //  ט
    case 10: return  '\xE0' + 9 ;  //  י
    case 11: return  '\xE0' + 11;  //  כ
    case 12: return  '\xE0' + 12;  //  ל
    case 13: return  '\xE0' + 14;  //  מ
    case 14: return  '\xE0' + 16;  //  נ
    case 15: return  '\xE0' + 17;  //  ס
    case 16: return  '\xE0' + 18;  //  ע
    case 17: return  '\xE0' + 20;  //  פ
    case 18: return  '\xE0' + 22;  //  צ
    case 19: return  '\xE0' + 23;  //  ק
    case 20: return  '\xE0' + 24;  //  ר
    case 21: return  '\xE0' + 25;  //  ש
    case 22: return  '\xE0' + 26;  //  ת
    default: throw std::invalid_argument("number out of [1, 26] range");
  }
}
