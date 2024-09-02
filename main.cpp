#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>

#include "american_english.hpp"
#include "phonology.hpp"

int main(int argc, char* argv[]) {
  int num_words = 100;
  if (argc >= 2) {
    num_words = std::stoi(argv[1]);
  }
  int max_num_syllables = 4;
  if (argc == 3) {
    max_num_syllables = std::stoi(argv[2]);
  }
  srand(time(nullptr));
  AmericanEnglish ae;
  for (int i = 0; i < num_words; ++i) {
    std::cout << phonology::get_word(ae.get_system(), max_num_syllables)
              << "\n";
  }
  return 0;
}
