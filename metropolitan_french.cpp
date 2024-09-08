#include "metropolitan_french.hpp"

#include <cstddef>
#include <cstdlib>
#include <ranges>
#include <string>
#include <vector>

#include "phonology.hpp"

namespace phonology {

static bool before_i_or_e(Spelling::RuleParams rp) {
  return rp.next && rp.next->vowel && rp.next->rounded == VR::UNROUNDED &&
         (rp.next->height == VH::CLOSE || rp.next->height == VH::CLOSE_MID ||
          rp.next->height == VH::MID || rp.next->height == VH::OPEN_MID);
}

static bool not_before_i_or_e(Spelling::RuleParams rp) { return !before_i_or_e(rp); }

void MetropolitanFrench::init_phonemes() {
  using enum IPA;

  phonemes.emplace_back(get_phone(i),
                        std::vector<Spelling>{{"i", any_position}, {"ie", word_final}});
  phonemes.emplace_back(
      get_phone(y),
      std::vector<Spelling>{{"u", any_position}, {"û", mid_word}, {"ue", word_final}});
  phonemes.emplace_back(
      get_phone(e),
      std::vector<Spelling>({{"é", any_position}, {"e", mid_word}, {"er", word_final}}));
  phonemes.emplace_back(
      get_phone(ø),
      std::vector<Spelling>({{"eu", any_position},
                             {"eû", [](Spelling::RuleParams rp) { return !word_final(rp); }},
                             {"œu", mid_word}}));
  phonemes.emplace_back(
      get_phone(œ),
      std::vector<Spelling>(
          {{"eu", any_position},
           {"eû", [](Spelling::RuleParams rp) { return !word_final(rp); }},
           {"œu",
            [](Spelling::RuleParams rp) { return mid_word(rp) && rp.prev->symbol != IPA::j; }},
           {"œ", mid_word}}));

  phonemes.emplace_back(
      get_phone(a),
      std::vector<Spelling>(
          {{"a", [](Spelling::RuleParams rp) { return !rp.prev || (rp.prev->symbol != IPA::w); }},
           {"à", [](Spelling::RuleParams rp) { return !rp.prev || (rp.prev->symbol != IPA::w); }},
           {"â",
            [](Spelling::RuleParams rp) {
              return (!rp.prev || (rp.prev->symbol != IPA::w)) && rp.next;
            }},
           {"", [](Spelling::RuleParams rp) { return rp.prev && (rp.prev->symbol == IPA::w); }}}));

  phonemes.emplace_back(get_phone(ɔ), std::vector<Spelling>({{"o", any_position}}));

  phonemes.emplace_back(get_phone(o), std::vector<Spelling>({{"au", any_position},
                                                             {"eau", any_position},
                                                             {"o", any_position},
                                                             {"ô", not_word_final}}));

  phonemes.emplace_back(
      get_phone(u),
      std::vector<Spelling>({{"ou", any_position}, {"oû", not_word_final}, {"oue", word_final}}));

  phonemes.emplace_back(get_phone(ɛ), std::vector<Spelling>({{"e", any_position},
                                                             {"ai", any_position},
                                                             {"aî", not_word_final},
                                                             {"è", mid_word},
                                                             {"ê", not_word_final},
                                                             {"ei", mid_word}}));

  phonemes.emplace_back(get_phone(ə), std::vector<Spelling>({{"e", any_position}}));

  auto not_before_glide = [](Spelling::RuleParams rp) {
    return !rp.prev ||
           (rp.prev->symbol != IPA::w && rp.prev->symbol != IPA::ɥ && rp.prev->symbol != IPA::j);
  };
  phonemes.emplace_back(
      get_phone(ɛ̃),
      std::vector<Spelling>(
          {{"ain", not_before_glide},
           {"aim", not_before_glide},
           {"um", not_before_glide},
           {"un", not_before_glide},
           {"ain", not_before_glide},
           {"ein", not_before_glide},
           {"im",
            [](Spelling::RuleParams rp) {
              return !rp.prev || (rp.prev->symbol != IPA::j && rp.prev->symbol != IPA::w);
            }},
           {"in",
            [](Spelling::RuleParams rp) {
              return !rp.prev || (rp.prev->symbol != IPA::j && rp.prev->symbol != IPA::w);
            }},
           {"în",
            [](Spelling::RuleParams rp) {
              return rp.prev && (rp.prev->symbol != IPA::j && rp.prev->symbol != IPA::w);
            }},
           {"en", [](Spelling::RuleParams rp) { return rp.prev && rp.prev->symbol == IPA::j; }},
           {"oin", [](Spelling::RuleParams rp) { return rp.prev && rp.prev->symbol == IPA::w; }}}));

  phonemes.emplace_back(get_phone(ɔ̃),
                        std::vector<Spelling>({{"on", any_position}, {"om", any_position}}));

  phonemes.emplace_back(get_phone(ɑ̃), std::vector<Spelling>({{"an", any_position},
                                                             {"am", any_position},
                                                             {"en", any_position},
                                                             {"em", any_position}}));

  phonemes.emplace_back(
      get_phone(m),
      std::vector<Spelling>({{"m",
                              [](Spelling::RuleParams rp) {
                                if (rp.word_final) {
                                  return false;
                                }
                                if (rp.prev && rp.prev->vowel && rp.next->vowel) {
                                  return false;
                                }
                                return true;
                              }},
                             {"mm",
                              [](Spelling::RuleParams rp) {
                                return (rp.prev && rp.next && rp.prev->vowel && rp.next->vowel);
                              }},
                             {"me", word_final},
                             {"mme", word_final}}));

  phonemes.emplace_back(
      get_phone(n),
      std::vector<Spelling>({{"n",
                              [](Spelling::RuleParams rp) {
                                if (rp.word_final) {
                                  return false;
                                }
                                if (rp.prev && rp.prev->vowel && rp.next->vowel) {
                                  return false;
                                }
                                return true;
                              }},
                             {"nn",
                              [](Spelling::RuleParams rp) {
                                return (rp.prev && rp.next && rp.prev->vowel && rp.next->vowel);
                              }},
                             {"ne", word_final},
                             {"nne", word_final}}));

  phonemes.emplace_back(get_phone(ɲ),
                        std::vector<Spelling>({{"gn", not_word_final}, {"gne", word_final}}));

  phonemes.emplace_back(
      get_phone(p),
      std::vector<Spelling>({{"p", not_word_final}, {"pp", between_vowels}, {"pe", word_final}}));

  phonemes.emplace_back(get_phone(t), std::vector<Spelling>({{"t", not_word_final},
                                                             {"tt", between_vowels},
                                                             {"te", word_final},
                                                             {"tte", [](Spelling::RuleParams rp) {
                                                                return word_final(rp) &&
                                                                       not_in_cluster(rp);
                                                              }}}));

  phonemes.emplace_back(
      get_phone(k),
      std::vector<Spelling>(
          {{"c",
            [](Spelling::RuleParams rp) {
              return not_before_i_or_e(rp) && (!word_final(rp) || not_in_cluster(rp));
            }},
           {"cc",
            [](Spelling::RuleParams rp) { return not_before_i_or_e(rp) && between_vowels(rp); }},
           {"qu", before_vowel},
           {"que", word_final}}));

  phonemes.emplace_back(
      get_phone(b),
      std::vector<Spelling>({{"b", not_word_final}, {"bb", between_vowels}, {"be", word_final}}));

  phonemes.emplace_back(
      get_phone(d),
      std::vector<Spelling>({{"d", not_word_final}, {"dd", between_vowels}, {"de", word_final}}));

  phonemes.emplace_back(
      get_phone(g),
      std::vector<Spelling>(
          {{"g",
            [](Spelling::RuleParams rp) { return not_before_i_or_e(rp) && not_word_final(rp); }},
           {"gu", before_i_or_e},
           {"gg",
            [](Spelling::RuleParams rp) { return not_before_i_or_e(rp) && between_vowels(rp); }},
           {"gue", word_final}}));

  phonemes.emplace_back(get_phone(f), std::vector<Spelling>({{"f", any_position},
                                                             {"ph", not_word_final},
                                                             {"ff", between_vowels},
                                                             {"fe", word_final},
                                                             {"phe", word_final}}));

  phonemes.emplace_back(
      get_phone(s),
      std::vector<Spelling>(
          {{"s", not_word_final},
           {"ç",
            [](Spelling::RuleParams rp) {
              return not_in_cluster(rp) && not_before_i_or_e(rp) && not_word_final(rp);
            }},
           {"c", [](Spelling::RuleParams rp) { return not_in_cluster(rp) && before_i_or_e(rp); }},
           {"ss", between_vowels},
           {"se", word_final},
           {"sse", [](Spelling::RuleParams rp) { return word_final(rp) && not_in_cluster(rp); }},
           {"ce", [](Spelling::RuleParams rp) { return word_final(rp) && not_in_cluster(rp); }}}));

  phonemes.emplace_back(get_phone(ʃ),
                        std::vector<Spelling>({{"ch", not_word_final}, {"che", word_final}}));

  phonemes.emplace_back(get_phone(v),
                        std::vector<Spelling>({{"v", not_word_final}, {"ve", word_final}}));

  phonemes.emplace_back(
      get_phone(z),
      std::vector<Spelling>({{"z", not_word_final}, {"s", mid_word}, {"se", word_final}}));

  phonemes.emplace_back(get_phone(ʒ), std::vector<Spelling>({{"j",
                                                              [](Spelling::RuleParams rp) {
                                                                return not_before_i_or_e(rp) &&
                                                                       not_word_final(rp);
                                                              }},
                                                             {"g", before_i_or_e},
                                                             {"ge", word_final}}));

  phonemes.emplace_back(
      get_phone(l),
      std::vector<Spelling>(
          {{"l", [](Spelling::RuleParams rp) { return not_word_final(rp) || not_in_cluster(rp); }},
           {"ll", between_vowels},
           {"le", word_final},
           {"lle", [](Spelling::RuleParams rp) { return word_final(rp) && not_in_cluster(rp); }}}));

  phonemes.emplace_back(get_phone(ʁ̞), std::vector<Spelling>({{"r", not_word_final},
                                                             {"rr", between_vowels},
                                                             {"re", word_final},
                                                             {"rre", [](Spelling::RuleParams rp) {
                                                                return word_final(rp) &&
                                                                       not_in_cluster(rp);
                                                              }}}));

  phonemes.emplace_back(
      get_phone(j), std::vector<Spelling>(
                        {{"i", [](Spelling::RuleParams rp) { return not_word_initial(rp); }},
                         {"y", word_initial},
                         {"il",
                          [](Spelling::RuleParams rp) {
                            return rp.prev && rp.prev->vowel && (rp.prev->backness == VB::FRONT);
                          }},
                         {"ille", [](Spelling::RuleParams rp) {
                            return rp.prev && !rp.next && rp.prev->vowel &&
                                   (rp.prev->backness == VB::FRONT);
                          }}}));

  phonemes.emplace_back(get_phone(ɥ),
                        std::vector<Spelling>({{"u", not_word_initial}, {"hu", word_initial}}));

  phonemes.emplace_back(get_phone(w), std::vector<Spelling>({
                                          {"oi", any_position},
                                      }));

  // clang-format on
}

void MetropolitanFrench::init_onsets() {
  auto consonants = std::views::all(phonemes) | std::views::filter(consonant);
  {
    // All single-consonant phonemes except /ɲ/
    onsets.emplace_back();
    auto candidates = consonants | std::views::filter(except(IPA::ɲ));
    for (const auto& c : candidates) {
      onsets.back().emplace_back();
      onsets.back().back().push_back(&c);
    }
  }
  {
    // Stop, or non-sibilant fricative, plus /r/
    onsets.emplace_back();
    auto start = consonants |
                 std::views::filter([](const auto& p) { return fricative(p) || stop(p); }) |
                 std::views::filter([](const auto& p) { return !sibilant(p); });
    auto r = get_phoneme(IPA::ʁ̞);
    for (const auto& c : start) {
      onsets.back().emplace_back();
      onsets.back().back().push_back(&c);
      onsets.back().back().push_back(r);
    }
  }
  {
    // Bilabial or velar stop, or /f/, plus /l/
    onsets.emplace_back();
    auto start = consonants | std::views::filter([](const auto& p) {
                   return (stop(p) && (p.p.poa == PoA::LABIAL || p.p.poa == PoA::VELAR)) ||
                          p.p.symbol == IPA::f;
                 });
    auto l = get_phoneme(IPA::l);
    for (const auto& c : start) {
      onsets.back().emplace_back();
      onsets.back().back().push_back(&c);
      onsets.back().back().push_back(l);
    }
  }
  {
    // /s/ plus voiceless stop or nasal except /ɲ/
    onsets.emplace_back();
    auto s = get_phoneme(IPA::s);
    auto candidates =
        consonants |
        std::views::filter([](const auto& p) { return nasal_c(p) || (stop(p) && voiceless(p)); }) |
        std::views::filter(except(IPA::ɲ));
    for (const auto& c : candidates) {
      onsets.back().emplace_back();
      onsets.back().back().push_back(s);
      onsets.back().back().push_back(&c);
    }
  }
  {
    // Consonant plus /w/
    onsets.emplace_back();
    auto candidates =
        consonants | std::views::filter(except({IPA::ɲ, IPA::g, IPA::z, IPA::j, IPA::ɥ, IPA::w}));
    auto w = get_phoneme(IPA::w);
    for (const auto& c : candidates) {
      onsets.back().emplace_back();
      onsets.back().back().push_back(&c);
      onsets.back().back().push_back(w);
    }
  }
  {
    // Consonant plus /ɥ/
    onsets.emplace_back();
    auto candidates =
        consonants | std::views::filter(except({IPA::ɲ, IPA::g, IPA::z, IPA::j, IPA::ɥ, IPA::w}));
    auto ɥ = get_phoneme(IPA::ɥ);
    for (const auto& c : candidates) {
      onsets.back().emplace_back();
      onsets.back().back().push_back(&c);
      onsets.back().back().push_back(ɥ);
    }
  }
  {
    // Consonant plus /j/
    onsets.emplace_back();
    auto candidates =
        consonants |
        std::views::filter(except({IPA::ɲ, IPA::g, IPA::z, IPA::ʒ, IPA::j, IPA::ɥ, IPA::w}));
    auto j = get_phoneme(IPA::j);
    for (const auto& c : candidates) {
      onsets.back().emplace_back();
      onsets.back().back().push_back(&c);
      onsets.back().back().push_back(j);
    }
  }
  {
    // /p/ plus /s/
    onsets.emplace_back();
    onsets.back().emplace_back();
    onsets.back().back().push_back(get_phoneme(IPA::p));
    onsets.back().back().push_back(get_phoneme(IPA::s));
  }
  {
    // TODO: three consonant clusters
  }
}

void MetropolitanFrench::init_nuclei() {
  {
    // Following unrestricted onset
    nuclei.emplace_back();
    auto candidates = std::views::all(phonemes) | std::views::filter(vowel);
    for (const auto& c : candidates) {
      nuclei.back().push_back(&c);
    }
  }
  {
    // Following /w/, only /a/ and /ɛ̃/
    // While other sequences are possible, e.g. oui (/wi/), ouais (/wɛ/),
    // these can be evaluated as /u/ + vowel, and so their spelling would look
    // correct, whereas there is no way to generate "oi" from /wa/
    nucleus_index_map[get_phoneme(IPA::w)] = nuclei.size();
    nuclei.emplace_back();
    nuclei.back().push_back(get_phoneme(IPA::a));
    nuclei.emplace_back();
    nuclei.back().push_back(get_phoneme(IPA::ɛ̃));
  }
  {
    // Following /ɥ/, only /i/
    nucleus_index_map[get_phoneme(IPA::ɥ)] = nuclei.size();
    nuclei.emplace_back();
    nuclei.back().push_back(get_phoneme(IPA::i));
  }
  {
    // Following /j/, only mid and close-mid front vowels except /œ/
    nucleus_index_map[get_phoneme(IPA::j)] = nuclei.size();
    nuclei.emplace_back();
    auto candidates = std::views::all(phonemes) | std::views::filter(front) |
                      std::views::filter(any_mid) | std::views::filter(except(IPA::œ));
    for (const auto& c : candidates) {
      nuclei.back().push_back(&c);
    }
  }
}

void MetropolitanFrench::init_codas() {
  auto consonants = std::views::all(phonemes) | std::views::filter(consonant);
  {
    // All single consonants except /w/ and /ɥ/
    codas.emplace_back();
    auto candidates = consonants | std::views::filter(except({IPA::w, IPA::ɥ}));
    for (auto& c : candidates) {
      codas.back().emplace_back();
      codas.back().back().push_back(&c);
    }
  }
  {
    // /r/ plus consonant except other approximants other than /l/
    codas.emplace_back();
    auto r = get_phoneme(IPA::ʁ̞);
    auto candidates = consonants | std::views::filter([](auto& p) {
                        return !approximant(p) || (p.p.symbol == IPA::l);
                      });
    for (auto& c : candidates) {
      codas.back().emplace_back();
      codas.back().back().push_back(r);
      codas.back().back().push_back(&c);
    }
  }
  {
    // /l/ plus consonant except other approximants or nasals other than /m/
    codas.emplace_back();
    auto l = get_phoneme(IPA::l);
    auto candidates =
        consonants | std::views::filter([](auto& p) { return !approximant(p); }) |
        std::views::filter([](auto& p) { return !nasal_c(p) || p.p.symbol == IPA::m; });
    for (const auto& c : candidates) {
      codas.back().emplace_back();
      codas.back().back().push_back(l);
      codas.back().back().push_back(&c);
    }
  }
  {
    // /s/ plus voiceless stop or /m/
    codas.emplace_back();
    auto s = get_phoneme(IPA::s);
    auto candidates = consonants | std::views::filter([](const auto& p) {
                        return (voiceless(p) && stop(p)) || p.p.symbol == IPA::m;
                      });
    for (const auto& c : candidates) {
      codas.back().emplace_back();
      codas.back().back().push_back(s);
      codas.back().back().push_back(&c);
    }
  }
  {
    // Stop or bilabial fricative plus /r/
    codas.emplace_back();
    auto candidates = consonants | std::views::filter([](const auto& p) {
                        return stop(p) || (labial(p) && fricative(p));
                      });
    auto r = get_phoneme(IPA::ʁ̞);
    for (const auto& c : candidates) {
      codas.back().emplace_back();
      codas.back().back().push_back(&c);
      codas.back().back().push_back(r);
    }
  }
  {
    // Non-alveolar stop or /f/ plus /l/
    codas.emplace_back();
    auto candidates = consonants | std::views::filter([](const auto& p) {
                        return (stop(p) && (p.p.poa != PoA::ALVEOLAR)) || (p.p.symbol == IPA::f);
                      });
    auto l = get_phoneme(IPA::l);
    for (const auto& c : candidates) {
      codas.back().emplace_back();
      codas.back().back().push_back(&c);
      codas.back().back().push_back(l);
    }
  }
  {
    // Non-alveolar voiceless stop plus voiceless alveolar
    codas.emplace_back();
    auto start = consonants | std::views::filter(voiceless) | std::views::filter(stop) |
                 std::views::filter([](const auto& p) { return !alveolar(p); });
    auto last = consonants | std::views::filter(voiceless) | std::views::filter(alveolar);
    for (const auto& s : start) {
      for (const auto& l : last) {
        codas.back().emplace_back();
        codas.back().back().push_back(&s);
        codas.back().back().push_back(&l);
      }
    }
  }
  {
    // After nasal vowel, only stops and non-labial fricatives
    auto nasals = consonants | std::views::filter(nasal_v);
    for (const auto& n : nasals) {
      coda_index_map[&n] = codas.size();
    }
    auto f = std::views::filter([](const std::vector<const Phoneme*>& p) {
      return stop(*p.front()) || (fricative(*p.front()) && !labial(*p.front()));
    });
    codas.emplace_back();
    for (auto it = codas.cbegin(); it != codas.cend() - 1; ++it) {
      for (const auto& c : std::views::all(*it) | f) {
        codas.back().emplace_back(c);
      }
    }
  }
}

std::vector<const Phoneme*> MetropolitanFrench::get_onset() const {
  std::vector<const Phoneme*> onset;
  int i = rand() % onsets.size();
  int j = rand() % onsets[i].size();
  for (auto p : onsets[i][j]) {
    onset.push_back(p);
  }
  return onset;
}

const Phoneme* MetropolitanFrench::get_nucleus(const Phoneme* onset) const {
  if (auto it = nucleus_index_map.find(onset); it != nucleus_index_map.end()) {
    std::size_t i = it->second;
    std::size_t j = rand() % nuclei[i].size();
    return nuclei[i][j];
  }
  std::size_t i = rand() % nuclei.front().size();
  return nuclei.front()[i];
}

std::vector<const Phoneme*> MetropolitanFrench::get_coda(const Phoneme* nucleus) const {
  if (rand() % 2) {
    return {};
  }
  std::vector<const Phoneme*> coda;
  std::size_t i = rand() % codas.size();
  if (auto it = coda_index_map.find(nucleus); it != coda_index_map.end()) {
    i = it->second;
  }
  int j = rand() % codas[i].size();
  for (auto p : codas[i][j]) {
    coda.push_back(p);
  }
  return coda;
}

std::string MetropolitanFrench::get_spelling(const Syllable& syllable, bool word_final) const {
  std::string x;
  Spelling::RuleParams rp;
  rp.prev = nullptr;
  rp.word_final = false;
  for (std::size_t i = 0; i < syllable.onset.size(); ++i) {
    rp.next = i == syllable.onset.size() - 1 ? &syllable.nucleus->p : &syllable.onset[i + 1]->p;
    x += syllable.onset[i]->GetSpelling(rp);
    rp.prev = &syllable.onset[i]->p;
  }

  if (syllable.coda.size()) {
    rp.next = &syllable.coda.front()->p;
  } else {
    rp.next = nullptr;
    rp.word_final = word_final;
  }
  x += syllable.nucleus->GetSpelling(rp);
  rp.prev = &syllable.nucleus->p;

  if (word_final && !syllable.coda.size()) {
    if (rand() % 2) {
      int i = rand() % silent_final_letters.size();
      x += silent_final_letters[i];
    }
  }

  for (std::size_t i = 0; i < syllable.coda.size(); ++i) {
    if (i == syllable.coda.size() - 1) {
      rp.next = nullptr;
      rp.word_final = word_final;
    } else {
      rp.next = &syllable.coda[i + 1]->p;
    }
    x += syllable.coda[i]->GetSpelling(rp);
    rp.prev = &syllable.coda[i]->p;
  }

  return x;
}

}  // namespace phonology
