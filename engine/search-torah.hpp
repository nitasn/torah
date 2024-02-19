#pragma once

#include "boyer-moore-els.hpp"

#include <string>

// apply boyer-moore-els on the torah
std::optional<SearchResult> search_torah(const std::string& pattern_utf8, size_t min_step = 0, size_t max_step = 0);

std::optional<SearchResult> search_torah_parallel(const std::string& pattern_utf8, size_t min_step = 0, size_t max_step = 0, int num_threads = 4);