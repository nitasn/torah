#include "boyer-moore-els.hpp"

#include <stdbool.h> // for bool
#include <unistd.h> // for ssize_t

/**
 * this is wikipedia's impl of boyer-moore substring search algorithm
 * [https://en.wikipedia.org/wiki/Boyer–Moore_string-search_algorithm#C_implementation]
 * adjusted for ELS (ELS = Equidistant Letter Sequence, a.k.a "the bible code")
 */

#define max(a, b) ((a < b) ? b : a)

#define ALPHABET_LEN (22 + 1) /* todo make it 22 */

void make_delta1(ptrdiff_t *delta1, uint8_t *pat, size_t patlen) {
  for (int i = 0; i < ALPHABET_LEN; i++) {
    delta1[i] = patlen;
  }
  for (int i = 0; i < patlen; i++) {
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

uint8_t *boyer_moore_els(
    uint8_t *string, size_t len_string, 
    uint8_t *pattern, size_t len_pattern, 
    size_t els_step // a.k.a the "diloog"
) {
  ptrdiff_t delta1[ALPHABET_LEN];
  ptrdiff_t delta2[len_pattern];
  make_delta1(delta1, pattern, len_pattern);
  make_delta2(delta2, pattern, len_pattern);

  if (len_pattern == 0) {
    return string;
  }

  for (size_t mod = 0; mod < els_step; ++mod) {
    size_t i = mod + els_step * (len_pattern - 1);
    while (i < len_string) {
      ptrdiff_t j = len_pattern - 1;
      while (j >= 0 && (string[i] == pattern[j])) {
        j -= 1;
        i -= els_step;
      }
      if (j < 0) {
        return &string[i + els_step];
      }

      i += els_step * max(delta1[string[i]], delta2[j]);
    }
  }

  return NULL;
}