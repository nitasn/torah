#include "read-file.hpp"

#include <fstream>
#include <iostream> // for std::cerr
#include <cstdlib> // for std::exit and EXIT_FAILURE

std::vector<uint8_t> readBinaryFile(const std::string& filename) {

  std::ifstream file(filename, std::ios::binary | std::ios::ate); // open file at the end

  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    std::exit(EXIT_FAILURE);
  }

  size_t size = file.tellg();
  file.seekg(0, std::ios::beg); // seek back to the beginning

  std::vector<uint8_t> buffer;
  buffer.resize(size);

  if (!file.read(reinterpret_cast<char *>(buffer.data()), size)) {
    std::cerr << "Failed to read the file: " << filename << std::endl;
    std::exit(EXIT_FAILURE);
  }

  file.close();

  return buffer;
}