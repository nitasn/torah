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