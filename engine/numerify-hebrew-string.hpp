#pragma once

#include <vector>

/**
 * This function takes a hebrew string in UTF-8 and returns a vector of numbers in the range [0, 21]
 * Whitespaces are skipped, and final letters (םןץףך) are merged with regular ones (מנצפכ)
 * 
 * E.g. 
 * "יצחק רבין" becomes [ 09, 17, 07, 18, 19, 01, 09, 13 ]
 * "אתה" becomes [ 00, 21, 04 ]
 */
std::vector<uint8_t> numerify_hebrew_string(const std::string &string_utf8);
