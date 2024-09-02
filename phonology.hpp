#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <functional>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

namespace phonology {

enum class IPA : uint8_t {
  ɑ,
  æ,
  aɪ,
  aʊ,
  ɛ,
  eɪ,
  ɪ,
  i,
  oʊ,
  ɔɪ,
  ʊ,
  ə,
  u,
  m,
  n,
  ŋ,
  p,
  t,
  tʃ,
  k,
  b,
  d,
  dʒ,
  g,
  f,
  θ,
  s,
  ʃ,
  h,
  v,
  ð,
  z,
  ʒ,
  w,
  l,
  r,
  j,
};

enum class PlaceOfArticulation : uint8_t {
  NONE,
  LABIAL,
  DENTAL,
  ALVEOLAR,
  POST_ALVEOLAR,
  PALATAL,
  VELAR,
  GLOTTAL,
};
using PoA = PlaceOfArticulation;

enum class MannerOfArticulation : uint8_t {
  NONE,
  NASAL,
  PLOSIVE,
  FRICATIVE,
  AFFRICATE,
  APPROXIMANT,
};
using MoA = MannerOfArticulation;

struct Spelling {
  const std::vector<std::string> onset;
  const std::vector<std::string> nucleus;
  const std::vector<std::string> coda;
  const std::vector<std::string> word_final;

  Spelling() = delete;

  // vowel constructor
  explicit Spelling(std::vector<std::string>&& nucleus) : nucleus(nucleus) {}

  // consonant constructor
  Spelling(std::vector<std::string>&& onset, std::vector<std::string>&& coda,
           std::vector<std::string>&& word_final)
      : onset(onset), coda(coda), word_final(word_final) {}
};

struct Phone {
  const IPA symbol;
  const bool vowel;
  const bool voiced;
  const MoA moa;
  const PoA poa;

  // vowel constructor
  explicit constexpr Phone(IPA symbol)
      : symbol(symbol),
        vowel(true),
        voiced(true),
        moa(MoA::NONE),
        poa(PoA::NONE) {}

  // consonant constructor
  constexpr Phone(IPA symbol, bool voiced, MannerOfArticulation moa,
                  PlaceOfArticulation poa)
      : symbol(symbol), vowel(false), voiced(voiced), moa(moa), poa(poa) {}
};

struct Phoneme {
  const Phone p;
  const Spelling spelling;

  Phoneme(Phone&& phone, Spelling&& spelling) : p(phone), spelling(spelling) {}

  std::string get_onset_spelling() const {
    int num_options = spelling.onset.size();
    assert(num_options);
    return spelling.onset[rand() % num_options];
  }

  std::string get_nucleus_spelling() const {
    int num_options = spelling.nucleus.size();
    assert(num_options);
    return spelling.nucleus[rand() % num_options];
  }

  std::string get_final_spelling() const {
    int num_options = spelling.word_final.size();
    assert(num_options);
    return spelling.word_final[rand() % num_options];
  }

  std::string get_coda_spelling(bool final) const {
    if (final) {
      return get_final_spelling();
    }
    int num_options = spelling.coda.size();
    assert(num_options);
    return spelling.coda[rand() % num_options];
  }
};

struct System {
  System() = default;
  explicit System(std::vector<Phoneme>&& phonemes) : phonemes(phonemes) {}

  const Phoneme* get_phoneme(IPA symbol) const {
    for (const auto& p : phonemes) {
      if (p.p.symbol == symbol) {
        return &p;
      }
    }
    return nullptr;
  }

  std::vector<Phoneme> phonemes;
  std::vector<std::vector<std::vector<const Phoneme*>>> onsets;
  std::vector<std::vector<const Phoneme*>> nuclei;
  std::vector<std::vector<std::vector<const Phoneme*>>> codas;
};

using phone_filter = std::function<bool(const Phoneme&)>;
// clang-format off
// Defining helper functions
inline bool consonant  (const Phoneme& p) { return !p.p.vowel; }
inline bool vowel      (const Phoneme& p) { return p.p.vowel; }
inline bool stop       (const Phoneme& p) { return p.p.moa == MoA::PLOSIVE; }
inline bool approximant(const Phoneme& p) { return p.p.moa == MoA::APPROXIMANT; }
inline bool fricative  (const Phoneme& p) { return p.p.moa == MoA::FRICATIVE; }
inline bool affricate  (const Phoneme& p) { return p.p.moa == MoA::AFFRICATE; }
inline bool nasal      (const Phoneme& p) { return p.p.moa == MoA::NASAL; }
inline bool voiced     (const Phoneme& p) { return p.p.voiced; }
inline bool voiceless  (const Phoneme& p) { return !p.p.voiced; }
inline bool sibilant   (const Phoneme& p) {
  return (p.p.moa == MoA::FRICATIVE) &&
         (p.p.poa == PoA::ALVEOLAR || p.p.poa == PoA::POST_ALVEOLAR);
}
// clang-format on

inline phone_filter except(const std::vector<phonology::IPA>& exceptions) {
  return [exceptions](const auto& p) {
    return std::find(exceptions.begin(), exceptions.end(), p.p.symbol) ==
           exceptions.end();
  };
}

inline phone_filter except(const phonology::IPA exception) {
  return [exception](const auto& p) { return p.p.symbol != exception; };
}

bool homorganic(const Phone* lhs, const Phone* rhs);
Phone get_phone(IPA symbol);
std::string get_word(const System& s, int max_num_syllables);

};  // namespace phonology
