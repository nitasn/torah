#include "iso-8859-8.hpp"

#include <iostream> // for std::cerr

uint8_t isoHebrew_to_number(char letter) {
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
      std::cerr << "isoHebrew_to_number failed: non iso-8859-8 hebrew letter" << '\n';
      exit(3);
  }
}

char number_to_isoHebrew(uint8_t number) {
  switch (number) {
    case 0 : return  '\xE0' + 0 ;  //  א
    case 1 : return  '\xE0' + 1 ;  //  ב
    case 2 : return  '\xE0' + 2 ;  //  ג
    case 3 : return  '\xE0' + 3 ;  //  ד
    case 4 : return  '\xE0' + 4 ;  //  ה
    case 5 : return  '\xE0' + 5 ;  //  ו
    case 6 : return  '\xE0' + 6 ;  //  ז
    case 7 : return  '\xE0' + 7 ;  //  ח
    case 8 : return  '\xE0' + 8 ;  //  ט
    case 9 : return  '\xE0' + 9 ;  //  י
    case 10: return  '\xE0' + 11;  //  כ
    case 11: return  '\xE0' + 12;  //  ל
    case 12: return  '\xE0' + 14;  //  מ
    case 13: return  '\xE0' + 16;  //  נ
    case 14: return  '\xE0' + 17;  //  ס
    case 15: return  '\xE0' + 18;  //  ע
    case 16: return  '\xE0' + 20;  //  פ
    case 17: return  '\xE0' + 22;  //  צ
    case 18: return  '\xE0' + 23;  //  ק
    case 19: return  '\xE0' + 24;  //  ר
    case 20: return  '\xE0' + 25;  //  ש
    case 21: return  '\xE0' + 26;  //  ת
    
    default: 
      std::cerr << "number_to_isoHebrew failed: number out of range [0, 21]" << '\n';
      exit(3);
  }
}
