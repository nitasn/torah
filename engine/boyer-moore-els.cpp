/**
 * This code is based on wikipedia's implementation of boyer-moore substring search algorithm
 * [https://en.wikipedia.org/wiki/Boyer–Moore_string-search_algorithm#C_implementation].
 * Below it's adjusted for ELS (Equidistant Letter Sequence) a.k.a "the bible code"
 */

#include "boyer-moore-els.hpp"

#include <stdbool.h> // for bool
#include <unistd.h> // for ssize_t

///////////////////////////////////////////////////////////////
///               M O D U L E   P R I V A T E               ///
///////////////////////////////////////////////////////////////

#define max(a, b) ((a < b) ? b : a)

#define ALPHABET_LEN 22

void make_delta1(ptrdiff_t *delta1, uint8_t *pat, size_t patlen) {
  for (int i = 0; i < ALPHABET_LEN; i++) {
    delta1[i] = patlen;
  }
  for (size_t i = 0; i < patlen; i++) {
    delta1[pat[i]] = patlen - 1 - i;
  }
}

bool is_prefix(uint8_t *word, size_t wordlen, ptrdiff_t pos) {
  int suffixlen = wordlen - pos;

  for (int i = 0; i < suffixlen; i++) {
    if (word[i] != word[pos + i]) {
      return false;
    }
  }
  return true;
}

size_t suffix_length(uint8_t *word, size_t wordlen, ptrdiff_t pos) {
  size_t i;

  for (i = 0; (word[pos - i] == word[wordlen - 1 - i]) && (i <= pos); i++)
    ;
  return i;
}

void make_delta2(ptrdiff_t *delta2, uint8_t *pat, size_t patlen) {
  ssize_t p;
  size_t last_prefix_index = 1;

  for (p = patlen - 1; p >= 0; p--) {
    if (is_prefix(pat, patlen, p + 1)) {
      last_prefix_index = p + 1;
    }
    delta2[p] = last_prefix_index + (patlen - 1 - p);
  }

  for (p = 0; p < patlen - 1; p++) {
    size_t slen = suffix_length(pat, patlen, p);
    if (pat[p - slen] != pat[patlen - 1 - slen]) {
      delta2[patlen - 1 - slen] = patlen - 1 - p + slen;
    }
  }
}

uint8_t *boyer_moore_els_impl(
    uint8_t *string, size_t len_string, 
    uint8_t *pattern, size_t len_pattern, 
    ptrdiff_t delta1[], ptrdiff_t delta2[],
    size_t step // a.k.a the "diloog"
) {

  for (size_t mod = 0; mod < step; ++mod) {
    size_t i = mod + step * (len_pattern - 1);
    while (i < len_string) {
      ptrdiff_t j = len_pattern - 1;
      while (j >= 0 && (string[i] == pattern[j])) {
        j -= 1;
        i -= step;
      }
      if (j < 0) {
        return &string[i + step];
      }

      i += step * max(delta1[string[i]], delta2[j]);
    }
  }

  return NULL;
}


///////////////////////////////////////////////////////////////
///                M O D U L E   E X P O R T                ///
///////////////////////////////////////////////////////////////

#include <string.h> // for "memchr"

/**
  Issue:
  The current implementation returns the first result in the lowest step;
  That is, a result at a lower index, but with a higher step, is possible.

  The current implementation is: 
    foreach step (low to high) -> foreach mod (low to high) -> search match index (low to high).
  Hence,
  The code may return a result at some index, 
  even though a result at a lower index is possible (with a higher mod or a higher step).

  For instance:
  In the text below, if we look for 'א ה מ' with step 2,
  it's found at index 14 (which has mod 0), but there's also a match at index 9 (which has mode 1)

  27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9  8  7  6  5  4  3  2  1  0
  ב  ר  א  ש  י  ת  ב  ר  א  א  ל  ה  י  מ  א  ת  ה  ש  מ  י  מ  ו  א  ת  ה  א  ר  ץ
*/

/**
 * TODO: impl negative steps (could be done trivially by re-searching for the reverse of the pattern)
 * 
 * TODO: return all results?
 */

uint8_t *boyer_moore_els(
    uint8_t *string, size_t len_string, 
    uint8_t *pattern, size_t len_pattern, 
    size_t *ptr_step
) {

  if (len_pattern == 0) {
    return string;
  }

  if (len_pattern == 1) {
    return (uint8_t *) memchr(string, *pattern, len_string);
  }

  ptrdiff_t delta1[ALPHABET_LEN];
  make_delta1(delta1, pattern, len_pattern);
  
  ptrdiff_t delta2[len_pattern];
  make_delta2(delta2, pattern, len_pattern);

  if (*ptr_step) {
    return boyer_moore_els_impl(string, len_string, pattern, len_pattern, delta1, delta2, *ptr_step);
  }

  const size_t max_step = (len_string - 1) / (len_pattern - 1); // see comment below

  for (size_t step = 1; step <= max_step; ++step) {
    uint8_t *match = boyer_moore_els_impl(string, len_string, pattern, len_pattern, delta1, delta2, step);
    if (match) {
      *ptr_step = step;
      return match;
    }
  }

  return NULL;
}

/*
the largest possible step size
occurs when the pattern spans the entire string:

0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15   (the big string)
x              x              x              x    (where pattern fits)

last_string_index = 15
len_pattern = 4
step = 5

we want:

last_pattern_index ≤ last_string_index

hence:

5    * 3                 ≤ 15
step * (len_pattern - 1) ≤ last_string_index

step ≤ last_string_index / (len_pattern - 1)

step ≤ (len_str - 1) / (len_pattern - 1)

---
note: len_pattern ≥ 2;
the function starts by verifying it.
*/