#include "phonology.hpp"

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <vector>

namespace phonology {

static const std::unordered_map<IPA, const Phone> phones = {
    // clang-format off
    {IPA::ɑ,  Phone(IPA::ɑ)},  {IPA::æ,  Phone(IPA::æ)},
    {IPA::aɪ, Phone(IPA::aɪ)}, {IPA::aʊ, Phone(IPA::aʊ)},
    {IPA::ɛ,  Phone(IPA::ɛ)},  {IPA::eɪ, Phone(IPA::eɪ)},
    {IPA::ɪ,  Phone(IPA::ɪ)},  {IPA::i,  Phone(IPA::i)},
    {IPA::oʊ, Phone(IPA::oʊ)}, {IPA::ɔɪ, Phone(IPA::ɔɪ)},
    {IPA::ʊ,  Phone(IPA::ʊ)},  {IPA::ə,  Phone(IPA::ə)},
    {IPA::u,  Phone(IPA::u)},
    {IPA::m,  Phone(IPA::m,  true,  MoA::NASAL,       PoA::LABIAL)},
    {IPA::n,  Phone(IPA::n,  true,  MoA::NASAL,       PoA::ALVEOLAR)},
    {IPA::ŋ,  Phone(IPA::ŋ,  true,  MoA::NASAL,       PoA::VELAR)},
    {IPA::p,  Phone(IPA::p,  false, MoA::PLOSIVE,     PoA::LABIAL)},
    {IPA::t,  Phone(IPA::t,  false, MoA::PLOSIVE,     PoA::ALVEOLAR)},
    {IPA::tʃ, Phone(IPA::tʃ, false, MoA::AFFRICATE,   PoA::POST_ALVEOLAR)},
    {IPA::k,  Phone(IPA::k,  false, MoA::PLOSIVE,     PoA::VELAR)},
    {IPA::b,  Phone(IPA::b,  true,  MoA::PLOSIVE,     PoA::LABIAL)},
    {IPA::d,  Phone(IPA::d,  true,  MoA::PLOSIVE,     PoA::ALVEOLAR)},
    {IPA::dʒ, Phone(IPA::dʒ, true,  MoA::AFFRICATE,   PoA::POST_ALVEOLAR)},
    {IPA::g,  Phone(IPA::g,  true,  MoA::PLOSIVE,     PoA::VELAR)},
    {IPA::f,  Phone(IPA::f,  false, MoA::FRICATIVE,   PoA::LABIAL)},
    {IPA::θ,  Phone(IPA::θ,  false, MoA::FRICATIVE,   PoA::DENTAL)},
    {IPA::s,  Phone(IPA::s,  false, MoA::FRICATIVE,   PoA::ALVEOLAR)},
    {IPA::ʃ,  Phone(IPA::ʃ,  false, MoA::FRICATIVE,   PoA::POST_ALVEOLAR)},
    {IPA::h,  Phone(IPA::h,  false, MoA::FRICATIVE,   PoA::GLOTTAL)},
    {IPA::v,  Phone(IPA::v,  true,  MoA::FRICATIVE,   PoA::LABIAL)},
    {IPA::ð,  Phone(IPA::ð,  true,  MoA::FRICATIVE,   PoA::DENTAL)},
    {IPA::z,  Phone(IPA::z,  true,  MoA::FRICATIVE,   PoA::ALVEOLAR)},
    {IPA::ʒ,  Phone(IPA::ʒ,  true,  MoA::FRICATIVE,   PoA::POST_ALVEOLAR)},
    {IPA::w,  Phone(IPA::w,  true,  MoA::APPROXIMANT, PoA::LABIAL)},
    {IPA::l,  Phone(IPA::l,  true,  MoA::APPROXIMANT, PoA::ALVEOLAR)},
    {IPA::r,  Phone(IPA::r,  true,  MoA::APPROXIMANT, PoA::POST_ALVEOLAR)},
    {IPA::j,  Phone(IPA::j,  true,  MoA::APPROXIMANT, PoA::PALATAL)},
    // clang-format on
};

// Private functions

static std::string get_onset(const phonology::System& s) {
  auto i = rand() % s.onsets.size();
  auto j = rand() % s.onsets[i].size();
  std::string onset;
  for (const auto& phoneme : s.onsets[i][j]) {
    onset += phoneme->get_onset_spelling();
  }
  return onset;
}

static std::string get_nucleus(const phonology::System& s) {
  auto i = rand() % s.nuclei.size();
  auto j = rand() % s.nuclei[i].size();
  return s.nuclei[i][j]->get_nucleus_spelling();
}

static std::string get_coda(const phonology::System& s, bool final) {
  auto i = rand() % s.codas.size();
  auto j = rand() % s.codas[i].size();
  std::string coda;
  for (std::size_t k = 0; k < s.codas[i][j].size() - 1; ++k) {
    coda += s.codas[i][j][k]->get_coda_spelling(false);
  }
  coda += s.codas[i][j].back()->get_coda_spelling(final);
  return coda;
}

static std::string get_syllable(const phonology::System& s, bool onset,
                                bool coda, bool final) {
  std::string syllable;
  if (onset) syllable += get_onset(s);
  syllable += get_nucleus(s);
  if (coda) syllable += get_coda(s, final);
  return syllable;
}

// Public functions

bool homorganic(const Phone* lhs, const Phone* rhs) {
  if (lhs->poa == rhs->poa) {
    return true;
  }
  return (lhs->poa == PoA::ALVEOLAR && rhs->poa == PoA::POST_ALVEOLAR) ||
         (lhs->poa == PoA::POST_ALVEOLAR && rhs->poa == PoA::ALVEOLAR);
}

Phone get_phone(IPA symbol) { return phones.at(symbol); }

std::string get_word(const phonology::System& s) {
  return get_syllable(s, true, false, false) +
         get_syllable(s, true, true, true);
}

}  // namespace phonology