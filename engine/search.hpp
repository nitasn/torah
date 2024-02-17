#pragma once

#include <string>
#include <optional>

struct SearchResult {
  size_t index, step;
};

// for wasm to export
bool search(const std::string& pattern_iso_8859_8, SearchResult* pSearchResult);

// for internal ergonomic use
std::optional<SearchResult> search(const std::string& pattern_iso_8859_8);