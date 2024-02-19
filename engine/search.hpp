#pragma once

#include <string>
#include <optional>

struct SearchResult {
  size_t index, step;
};

// for ergonomic use
std::optional<SearchResult> search(const std::string& pattern_utf8);