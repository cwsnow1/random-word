#pragma once
#include "phonology.hpp"

namespace phonology {
class AmericanEnglish : public System<AmericanEnglish> {
  friend class System;

  private:
  void init_phonemes();
  void init_onsets();
  void init_nuclei();
  void init_codas();
  std::vector<const Phoneme*> get_onset() const;
  const Phoneme* get_nucleus(const Phoneme* onset) const;
  std::vector<const Phoneme*> get_coda(const Phoneme* nucleus) const;

  std::string get_spelling(const Syllable& syllable, bool word_final) const;

};

}  // namespace phonology
