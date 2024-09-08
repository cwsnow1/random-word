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
  ɑ̃,
  æ,
  a,
  aɪ,
  aʊ,
  ɛ,
  ɛ̃,
  œ,
  e,
  eɪ,
  ø,
  ɪ,
  i,
  y,
  o,
  oʊ,
  ɔ,
  ɔ̃,
  ɔɪ,
  ʊ,
  ə,
  u,
  m,
  n,
  ɲ,
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
  ɹ,
  ɥ,
  ʁ̞,
  j
};

enum class VowelRoundedness : uint8_t {
  UNROUNDED,
  ROUNDED,
};
using VR = VowelRoundedness;

enum class VowelNasality : uint8_t {
  ORAL,
  NASAL,
};
using VN = VowelNasality;

enum class VowelHeight : uint8_t {
  CLOSE,
  NEAR_CLOSE,
  CLOSE_MID,
  MID,
  OPEN_MID,
  NEAR_OPEN,
  OPEN,
};
using VH = VowelHeight;

enum class VowelBackness : uint8_t {
  FRONT,
  CENTRAL,
  BACK,
};
using VB = VowelBackness;

enum class ConsonantVoicing : uint8_t {
  VOICELESS,
  VOICED,
};
using CV = ConsonantVoicing;

enum class PlaceOfArticulation : uint8_t {
  LABIAL,
  DENTAL,
  ALVEOLAR,
  POST_ALVEOLAR,
  PALATAL,
  VELAR,
  UVULAR,
  GLOTTAL,
};
using PoA = PlaceOfArticulation;

enum class MannerOfArticulation : uint8_t {
  NASAL,
  PLOSIVE,
  FRICATIVE,
  AFFRICATE,
  APPROXIMANT,
};
using MoA = MannerOfArticulation;

struct Phone {
  const IPA symbol;
  const bool vowel;
  union {
    struct {
      const CV voicing;
      const MoA moa;
      const PoA poa;
    };
    struct {
      const VR rounded;
      const VH height;
      const VB backness;
      const VN nasality;
    };
  };

  // vowel constructor
  constexpr Phone(IPA symbol, VowelRoundedness rounded, VowelHeight height, VowelBackness backness,
                  VowelNasality nasality)
      : symbol(symbol),
        vowel(true),
        rounded(rounded),
        height(height),
        backness(backness),
        nasality(nasality) {}

  // consonant constructor
  constexpr Phone(IPA symbol, ConsonantVoicing voicing, MannerOfArticulation moa,
                  PlaceOfArticulation poa)
      : symbol(symbol), vowel(false), voicing(voicing), moa(moa), poa(poa) {}
};

struct Spelling {
  struct RuleParams {
    const Phone* prev;
    const Phone* next;
    bool word_final;
  };

  using SpellingRule = std::function<bool(RuleParams)>;
  const std::string spelling;
  const SpellingRule rule;
  Spelling() = delete;
  Spelling(std::string&& spelling, SpellingRule&& rule) : spelling(spelling), rule(rule) {}
};

struct Phoneme {
  const Phone p;
  const std::vector<Spelling> spellings;

  Phoneme(Phone&& phone, std::vector<Spelling>&& spellings) : p(phone), spellings(spellings) {}

  std::string GetSpelling(Spelling::RuleParams p) const {
    std::size_t i = rand() % spellings.size();
    while (!spellings[i].rule(p)) {
      i = (i + 1) % spellings.size();
    }
    return spellings[i].spelling;
  }
};

struct Syllable {
  std::vector<const Phoneme*> onset;
  const Phoneme* nucleus;
  std::vector<const Phoneme*> coda;
};

template <class T>
class System {
 public:
  System() {
    static_cast<T*>(this)->init_phonemes();
    static_cast<T*>(this)->init_onsets();
    static_cast<T*>(this)->init_nuclei();
    static_cast<T*>(this)->init_codas();
  }

  const Phoneme* get_phoneme(IPA symbol) const {
    for (const auto& p : phonemes) {
      if (p.p.symbol == symbol) {
        return &p;
      }
    }
    assert(0);
    return nullptr;
  }

  // TODO: use inplace_vector or something similar
  std::vector<const Phoneme*> get_onset() const { return static_cast<const T*>(this)->get_onset(); }
  const Phoneme* get_nucleus(const Phoneme* onset) const {
    return static_cast<const T*>(this)->get_nucleus(onset);
  }
  std::vector<const Phoneme*> get_coda(const Phoneme* nucleus) const {
    return static_cast<const T*>(this)->get_coda(nucleus);
  }

  std::string get_spelling(const Syllable& syllable, bool word_final) const {
    return static_cast<const T*>(this)->get_spelling(syllable, word_final);
  }

 protected:
  std::vector<Phoneme> phonemes;
  std::vector<std::vector<std::vector<const Phoneme*>>> onsets;
  std::vector<std::vector<const Phoneme*>> nuclei;
  std::vector<std::vector<std::vector<const Phoneme*>>> codas;

  std::unordered_map<const Phoneme*, std::size_t> nucleus_index_map;
  std::unordered_map<const Phoneme*, std::size_t> coda_index_map;
};

using phone_filter = std::function<bool(const Phoneme&)>;
// clang-format off
// Defining helper functions
inline bool consonant  (const Phoneme& p) { return !p.p.vowel; }
inline bool vowel      (const Phoneme& p) { return p.p.vowel; }
inline bool rounded    (const Phoneme& p) { return p.p.vowel && p.p.rounded == VR::ROUNDED; }
inline bool unrounded  (const Phoneme& p) { return p.p.vowel && p.p.rounded == VR::UNROUNDED; }
inline bool front      (const Phoneme& p) { return p.p.vowel && p.p.backness == VB::FRONT; }
inline bool mid        (const Phoneme& p) { return p.p.vowel && p.p.height == VH::MID; }
inline bool nasal_v    (const Phoneme& p) { return p.p.vowel && p.p.nasality == VN::NASAL; }
inline bool oral_v     (const Phoneme& p) { return p.p.vowel && p.p.nasality == VN::ORAL; }
inline bool stop       (const Phoneme& p) { return !p.p.vowel && p.p.moa == MoA::PLOSIVE; }
inline bool approximant(const Phoneme& p) { return !p.p.vowel && p.p.moa == MoA::APPROXIMANT; }
inline bool fricative  (const Phoneme& p) { return !p.p.vowel && p.p.moa == MoA::FRICATIVE; }
inline bool affricate  (const Phoneme& p) { return !p.p.vowel && p.p.moa == MoA::AFFRICATE; }
inline bool nasal_c    (const Phoneme& p) { return !p.p.vowel && p.p.moa == MoA::NASAL; }
inline bool labial     (const Phoneme& p) { return !p.p.vowel && p.p.poa == PoA::LABIAL; }
inline bool alveolar   (const Phoneme& p) { return !p.p.vowel && p.p.poa == PoA::ALVEOLAR; }
inline bool voiced     (const Phoneme& p) { return !p.p.vowel && p.p.voicing == CV::VOICED; }
inline bool voiceless  (const Phoneme& p) { return !p.p.vowel && p.p.voicing == CV::VOICELESS; }
inline bool sibilant   (const Phoneme& p) {
  return (p.p.moa == MoA::FRICATIVE) &&
         (p.p.poa == PoA::ALVEOLAR || p.p.poa == PoA::POST_ALVEOLAR);
}
inline bool any_mid    (const Phoneme& p) {
  return p.p.vowel &&
        (p.p.height == VH::CLOSE_MID || p.p.height == VH::MID || p.p.height == VH::OPEN_MID);
}
// clang-format on

inline constexpr phone_filter except(const std::vector<phonology::IPA>& exceptions) {
  return [exceptions](const auto& p) {
    return std::find(exceptions.begin(), exceptions.end(), p.p.symbol) == exceptions.end();
  };
}

inline constexpr phone_filter except(const phonology::IPA exception) {
  return [exception](const auto& p) { return p.p.symbol != exception; };
}

inline bool any_position([[maybe_unused]] Spelling::RuleParams rp) { return true; }
inline bool word_final(Spelling::RuleParams rp) { return rp.word_final; };
inline bool not_word_final(Spelling::RuleParams rp) { return !rp.word_final; }
inline bool not_in_cluster(Spelling::RuleParams rp) {
  return (rp.prev == nullptr || rp.prev->vowel) && (rp.next == nullptr || rp.next->vowel);
}
inline bool word_initial(Spelling::RuleParams rp) { return rp.prev == nullptr; };
inline bool not_word_initial(Spelling::RuleParams rp) { return !word_initial(rp); }
inline bool between_vowels(Spelling::RuleParams rp) {
  return (rp.prev && !rp.word_final && rp.prev->vowel && (rp.next == nullptr));
}
inline bool before_vowel(Spelling::RuleParams rp) { return rp.next && rp.next->vowel; }
inline bool mid_word(Spelling::RuleParams rp) { return rp.prev && !rp.word_final; }

bool homorganic(const Phone* lhs, const Phone* rhs);
Phone get_phone(IPA symbol);

namespace {
template <class T>
static std::string get_syllable(const phonology::System<T>& s, bool final) {
  std::string syllable_string;
  Syllable syllable;
  syllable.onset = s.get_onset();
  syllable.nucleus = s.get_nucleus(syllable.onset.back());
  syllable.coda = s.get_coda(syllable.nucleus);
  return s.get_spelling(syllable, final);
}
}  // namespace

template <class T>
std::string get_word(const System<T>& s, int max_num_syllables) {
  int num_syllables = (rand() % max_num_syllables) + 1;
  std::string word;
  bool prev_onset = false;
  bool prev_coda = false;
  for (int i = 0; i < num_syllables; ++i) {
    bool coda = false;
    bool onset = !prev_coda;
    if (i == 0) {
      onset = rand() % 8;
    }
    if (!prev_onset) {
      coda = rand() % 2;
    }
    word += get_syllable(s, i == num_syllables - 1);
    prev_onset = onset;
    prev_coda = coda;
  }
  return word;
}

};  // namespace phonology
