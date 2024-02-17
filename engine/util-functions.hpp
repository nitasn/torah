#pragma once

#include <string>
#include <vector>

std::vector<uint8_t> removeSpacesAndConvertToNumbers(const std::string& string_iso_8859_8);

std::string utf8ToIso8859_8(const std::string& utf8String);