#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>

#include "metropolitan_french.hpp"
#include "phonology.hpp"

int main(int argc, char* argv[]) {
  int num_words = 100;
  if (argc >= 2) {
    num_words = std::stoi(argv[1]);
  }
  int max_num_syllables = 1;
  if (argc == 3) {
    max_num_syllables = std::stoi(argv[2]);
  }
  srand(time(nullptr));
  phonology::MetropolitanFrench mf;
  for (int i = 0; i < num_words; ++i) {
    std::cout << phonology::get_word(mf, max_num_syllables) << "\n";
  }
  return 0;
}
