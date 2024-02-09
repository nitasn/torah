#include "write-torah-block.hpp"
#include "util-functions.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

const std::vector<const char *> filenames = {
    "books-iso-8859-8/Bereshit.txt",
    "books-iso-8859-8/Shemot.txt",
    "books-iso-8859-8/Vayikra.txt",
    "books-iso-8859-8/Bamidbar.txt",
    "books-iso-8859-8/Devarim.txt",
};

void writeTorahBlockFile() {
  std::stringstream concat;

  // concat input files

  for (const auto& filename : filenames) {
    std::ifstream infile(filename);

    if (!infile.is_open()) {
      std::cerr << "Failed to open " << filename << std::endl;
      exit(EXIT_FAILURE);
    }

    concat << infile.rdbuf();

    infile.close();
  }

  auto result = removeSpacesAndConvertToNumbers(concat.str());

  // write output file

  std::cout << "writing torah.block (" << result.length() << " bytes)" << std::endl;

  std::ofstream outfile("torah.block");

  outfile << result;

  outfile.close();
}