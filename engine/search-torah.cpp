#include "search-torah.hpp"

#include "numerify-hebrew-string.hpp"
#include "torah_block.h"

std::optional<SearchResult> search_torah(const std::string& pattern_utf8, size_t min_step, size_t max_step) {
  std::vector<uint8_t> simplified_pat = numerify_hebrew_string(pattern_utf8);

  size_t max_possible_step = (torah_block_len - 1) / (simplified_pat.size() - 1);
  /** TODO: currently this logic is duplicated. but also, table generation is repeated for each thread! */
  if (min_step == 0) min_step = 1;
  if (max_step == 0 || max_step > max_possible_step) max_step = max_possible_step;

  return boyer_moore_els(
    torah_block, torah_block_len,
    simplified_pat.data(), simplified_pat.size(),
    min_step, max_step
  );
}

#include <iostream>
#include <vector>
#include <thread>
#include <optional>
#include <mutex>

std::optional<SearchResult> search_torah_parallel(const std::string& pattern_utf8, size_t min_step, size_t max_step, int num_threads) {
  std::vector<std::optional<SearchResult>> results(num_threads);
  std::vector<std::thread> threads; 

  std::vector<uint8_t> simplified_pat = numerify_hebrew_string(pattern_utf8);
  uint8_t *pattern = simplified_pat.data();
  size_t pattern_len = simplified_pat.size();

  size_t max_possible_step = (torah_block_len - 1) / (pattern_len - 1);
  /** TODO: currently this logic is duplicated */
  if (min_step == 0) min_step = 1;
  if (max_step == 0 || max_step > max_possible_step) max_step = max_possible_step;

  double _chunk_size = (double) (max_step + 1 - min_step) / num_threads;
  double _current_min = min_step;
  
  for (int i = 0; i < num_threads; ++i) {

    uint64_t _min_step = std::round(_current_min);
    uint64_t _max_step = std::round(_current_min += _chunk_size) - 1;
    
    if (i == num_threads - 1) _max_step = max_step;

    threads.emplace_back([=, &results] {
      results[i] = boyer_moore_els(torah_block, torah_block_len, pattern, pattern_len, _min_step, _max_step);
    });
  }

  for (auto& th : threads) {
    th.join();
  }

  for (const auto& result : results) {
    if (result) {
      return result;
    }
  }

  return std::nullopt;
}
