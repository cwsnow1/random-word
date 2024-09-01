#pragma once
#include "phonology.hpp"

class AmericanEnglish {
 public:
  AmericanEnglish();
  const phonology::System& get_system() { return system; }

 private:
  phonology::System system;
};