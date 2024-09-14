#include "american_english.hpp"

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

void AmericanEnglish::init_phonemes() {
  using enum IPA;
  phonemes.emplace_back(get_phone(æ), std::vector<Spelling>{{"a", any_position}});
  phonemes.emplace_back(get_phone(ɑ), std::vector<Spelling>{{"o", any_position},
                                                            {"al", mid_word},
                                                            {"au", not_word_final},
                                                            {"aw", any_position},
                                                            {"ough", word_final},
                                                            {"augh", word_final}});
  phonemes.emplace_back(get_phone(ɪ), std::vector<Spelling>{{"i", any_position}});
  phonemes.emplace_back(get_phone(ɛ), std::vector<Spelling>{{"e", any_position}, {"ea", mid_word}});
  phonemes.emplace_back(get_phone(ə), std::vector<Spelling>{{"a", any_position},
                                                            {"e", not_word_final},
                                                            {"o", not_word_final},
                                                            {"u", not_word_final},
                                                            {"ou", not_word_final}});
  phonemes.emplace_back(
      get_phone(ʊ),
      std::vector<Spelling>{{"u", not_word_final}, {"oo", mid_word}, {"o", mid_word}});
  phonemes.emplace_back(
      get_phone(eɪ),
      std::vector<Spelling>{{"a", mid_word}, {"ai", not_word_final}, {"ay", not_word_initial}});
  phonemes.emplace_back(
      get_phone(oʊ),
      std::vector<Spelling>{{"o", any_position}, {"oa", any_position}, {"ow", any_position}});
  phonemes.emplace_back(
      get_phone(i),
      std::vector<Spelling>{
          {"e", mid_word}, {"ea", any_position}, {"ee", not_word_initial}, {"y", word_final}});
  phonemes.emplace_back(get_phone(u), std::vector<Spelling>{{"u", not_word_final},
                                                            {"oo", not_word_initial},
                                                            {"ew", any_position},
                                                            {"ue", word_final}});
  phonemes.emplace_back(
      get_phone(aɪ), std::vector<Spelling>{
                         {"i", any_position}, {"y", not_word_initial}, {"igh", not_word_initial}});
  phonemes.emplace_back(get_phone(ɔɪ),
                        std::vector<Spelling>{{"oi", not_word_final}, {"oy", any_position}});
  phonemes.emplace_back(get_phone(aʊ),
                        std::vector<Spelling>{{"ou", not_word_final}, {"ow", any_position}});

  phonemes.emplace_back(
      get_phone(m),
      std::vector<Spelling>{
          {"m", any_position},
          {"mm", [](const Spelling::RuleParams& rp) { return not_in_cluster(rp) && is_coda(rp); }},
          {"me", word_final}});
  phonemes.emplace_back(
      get_phone(n),
      std::vector<Spelling>{
          {"n", any_position},
          {"nn", [](const Spelling::RuleParams& rp) { return not_in_cluster(rp) && is_coda(rp); }},
          {"ne", word_final}});
  phonemes.emplace_back(get_phone(ŋ),
                        std::vector<Spelling>{{"ng", not_in_cluster}, {"n", in_cluster}});

  phonemes.emplace_back(
      get_phone(p),
      std::vector<Spelling>{
          {"p", any_position},
          {"pp", [](const Spelling::RuleParams& rp) { return not_in_cluster(rp) && is_coda(rp); }},
          {"pe", word_final}});

  phonemes.emplace_back(
      get_phone(t),
      std::vector<Spelling>{
          {"t", any_position},
          {"tt", [](const Spelling::RuleParams& rp) { return not_in_cluster(rp) && is_coda(rp); }},
          {"te", word_final}});

  phonemes.emplace_back(get_phone(tʃ),
                        std::vector<Spelling>{{"ch", any_position}, {"tch", is_coda}});

  phonemes.emplace_back(get_phone(tʃ),
                        std::vector<Spelling>{{"ch", any_position}, {"tch", is_coda}});
  phonemes.emplace_back(
      get_phone(k),
      std::vector<Spelling>{
          {"c", not_before_i_or_e}, {"k", before_i_or_e}, {"ck", is_coda}, {"ke", word_final}});

  phonemes.emplace_back(
      get_phone(k),
      std::vector<Spelling>{
          {"c", not_before_i_or_e}, {"k", before_i_or_e}, {"ck", is_coda}, {"ke", word_final}});
  phonemes.emplace_back(
      get_phone(b),
      std::vector<Spelling>{
          {"b", any_position},
          {"bb", [](const Spelling::RuleParams& rp) { return not_in_cluster(rp) && is_coda(rp); }},
          {"be", word_final}});
  phonemes.emplace_back(
      get_phone(d),
      std::vector<Spelling>{
          {"d", any_position},
          {"dd", [](const Spelling::RuleParams& rp) { return not_in_cluster(rp) && is_coda(rp); }},
          {"de", word_final}});

  phonemes.emplace_back(
      get_phone(dʒ),
      std::vector<Spelling>{
          {"j", is_onset}, {"g", before_i_or_e}, {"ge", word_final}, {"dge", word_final}});
  phonemes.emplace_back(get_phone(g), std::vector<Spelling>{{"g", any_position}, {"gg", is_coda}});
  phonemes.emplace_back(
      get_phone(f),
      std::vector<Spelling>{
          {"f", [](Spelling::RuleParams rp) { return !(in_cluster(rp) && before_vowel(rp)); }},
          {"ph", [](Spelling::RuleParams rp) { return !(in_cluster(rp) && !before_vowel(rp)); }},
          {"fe", word_final}});

  phonemes.emplace_back(get_phone(θ), std::vector<Spelling>{{"th", any_position}});
  phonemes.emplace_back(
      get_phone(s),
      std::vector<Spelling>{
          {"s", any_position},
          {"ss", [](const Spelling::RuleParams& rp) { return not_in_cluster(rp) && is_coda(rp); }},
          {"ce", word_final}});
  phonemes.emplace_back(get_phone(ʃ), std::vector<Spelling>{{"sh", any_position}});
  phonemes.emplace_back(get_phone(v),
                        std::vector<Spelling>{{"v", not_word_final}, {"ve", word_final}});
  phonemes.emplace_back(get_phone(ð),
                        std::vector<Spelling>{{"th", any_position}, {"the", word_final}});
  phonemes.emplace_back(get_phone(z),
                        std::vector<Spelling>{{"z", any_position}, {"ze", word_final}});
  phonemes.emplace_back(get_phone(ʒ),
                        std::vector<Spelling>{{"j", is_onset}, {"si", mid_word}, {"ge", is_coda}});
  phonemes.emplace_back(get_phone(h), std::vector<Spelling>{{"h", any_position}});
  phonemes.emplace_back(get_phone(w), std::vector<Spelling>{{"w", any_position}});
  phonemes.emplace_back(
      get_phone(l),
      std::vector<Spelling>{
          {"l", any_position},
          {"ll", [](const Spelling::RuleParams& rp) { return not_in_cluster(rp) && is_coda(rp); }},
          {"le", word_final}});
  phonemes.emplace_back(get_phone(ɹ), std::vector<Spelling>{{"r", any_position}});
  phonemes.emplace_back(get_phone(j), std::vector<Spelling>{{"y", any_position}});
}

void AmericanEnglish::init_onsets() {
  {
    // All single-consonant phonemes except /ŋ/
    onsets.emplace_back();
    auto candidates = std::views::all(phonemes) | std::views::filter(consonant) |
                      std::views::filter(except(IPA::ŋ));
    for (const auto& c : candidates) {
      onsets.back().emplace_back();
      onsets.back().back().push_back(&c);
    }
  }

  {
    // Stop plus approximant other than /j/
    onsets.emplace_back();
    auto stops = std::views::all(phonemes) | std::views::filter(stop);
    auto approximants = std::views::all(phonemes) | std::views::filter(approximant) |
                        std::views::filter(except(IPA::j));
    for (const auto& s : stops) {
      for (const auto& a : approximants) {
        if (s.p.poa == a.p.poa) {
          continue;
        }
        onsets.back().emplace_back();
        onsets.back().back().push_back(&s);
        onsets.back().back().push_back(&a);
      }
    }
  }
  {  // Voiceless fricative except /h/ plus approximant other than /j/
     // Exception /s/ + /r/ is not possible
    onsets.emplace_back();
    auto fricatives = std::views::all(phonemes) | std::views::filter(fricative) |
                      std::views::filter(voiceless) | std::views::filter(except(IPA::h));
    auto approximants = std::views::all(phonemes) | std::views::filter(approximant) |
                        std::views::filter(except(IPA::j));
    for (const auto& f : fricatives) {
      for (const auto& a : approximants) {
        if (f.p.poa == a.p.poa || (f.p.symbol == IPA::s && a.p.symbol == IPA::ɹ)) {
          continue;
        }
        onsets.back().emplace_back();
        onsets.back().back().push_back(&f);
        onsets.back().back().push_back(&a);
      }
    }
  }
  {  // /s/ plus voiceless stop
    onsets.emplace_back();
    auto s = get_phoneme(IPA::s);
    auto stops =
        std::views::all(phonemes) | std::views::filter(stop) | std::views::filter(voiceless);
    for (const auto& plosive : stops) {
      onsets.back().emplace_back();
      onsets.back().back().push_back(s);
      onsets.back().back().push_back(&plosive);
    }
  }
  {  // /s/ plus nasal other than /ŋ/
    onsets.emplace_back();
    auto s = get_phoneme(IPA::s);
    auto nasals = std::views::all(phonemes) | std::views::filter(nasal_c) |
                  std::views::filter(except(IPA::ŋ));
    for (const auto& n : nasals) {
      onsets.back().emplace_back();
      onsets.back().back().push_back(s);
      onsets.back().back().push_back(&n);
    }
  }
  /*
  { // /s/ plus voiceless non-sibilant fricative
    onsets.emplace_back();
    auto s = get_phoneme(IPA::s);
    auto fricatives =
        std::views::all(phonemes) | std::views::filter(fricative) |
        std::views::filter(voiceless) |
        std::views::filter([](const auto& p) { return !sibilant(p); });
    for (const auto& f : fricatives) {
      onsets.back().emplace_back();
      onsets.back().back().push_back(s);
      onsets.back().back().push_back(&f);
    }
  }
  */
  {  // /s/ plus voiceless stop plus approximant except /r/
    onsets.emplace_back();
    auto s = get_phoneme(IPA::s);
    auto stops =
        std::views::all(phonemes) | std::views::filter(voiceless) | std::views::filter(stop);
    auto approximants = std::views::all(phonemes) | std::views::filter(approximant) |
                        std::views::filter(except(IPA::ɹ));
    for (const auto& plosive : stops) {
      for (const auto& a : approximants) {
        if (plosive.p.poa == a.p.poa) continue;
        onsets.back().emplace_back();
        onsets.back().back().push_back(s);
        onsets.back().back().push_back(&plosive);
        onsets.back().back().push_back(&a);
      }
    }
  }
}

void AmericanEnglish::init_nuclei() {
  {
    nuclei.emplace_back();
    auto candidates = std::views::all(phonemes) | std::views::filter(vowel);
    for (const auto& c : candidates) {
      nuclei.back().push_back(&c);
    }
  }
  nuclei_requiring_coda.insert(get_phoneme(IPA::ʊ));
}

void AmericanEnglish::init_codas() {
  {  // The single consonant phonemes except /h/, /w/, /j/
    codas.emplace_back();
    auto candidates = std::views::all(phonemes) | std::views::filter(consonant) |
                      std::views::filter(except({IPA::h, IPA::w, IPA::j}));
    for (const auto& c : candidates) {
      codas.back().emplace_back();
      codas.back().back().push_back(&c);
    }
  }
  {  // Lateral approximant plus stop or affricate: /lp/,
     // /lb/, /lt/, /ld/, /ltʃ/, /ldʒ/, /lk/
    codas.emplace_back();
    auto start = get_phoneme(IPA::l);
    auto candidates = std::views::all(phonemes) |
                      std::views::filter([](const auto& p) { return stop(p) || affricate(p); });
    for (const auto& c : candidates) {
      codas.back().emplace_back();
      codas.back().back().push_back(start);
      codas.back().back().push_back(&c);
    }
  }
  {  // In rhotic varieties, /r/ plus stop or affricate: /rp/,
     // /rb/, /rt/, /rd/, /rtʃ/, /rdʒ/, /rk/, /rɡ/
    codas.emplace_back();
    auto start = get_phoneme(IPA::ɹ);
    auto candidates = std::views::all(phonemes) |
                      std::views::filter([](const auto& p) { return stop(p) || affricate(p); });
    for (const auto& c : candidates) {
      codas.back().emplace_back();
      codas.back().back().push_back(start);
      codas.back().back().push_back(&c);
    }
  }
  {  // Lateral approximant + fricative except /h/: /lf/, /lv/,
     // /lθ/, /ls/, /lz/, /lʃ/, (/lð/)
    codas.emplace_back();
    auto start = get_phoneme(IPA::l);
    auto candidates = std::views::all(phonemes) | std::views::filter(fricative) |
                      std::views::filter(except(IPA::h));
    for (const auto& c : candidates) {
      codas.back().emplace_back();
      codas.back().back().push_back(start);
      codas.back().back().push_back(&c);
    }
  }
  {  // In rhotic varieties, /r/ + fricative: /rf/, /rv/, /rθ/, /rð/, /rs/,
     // /rz/, /rʃ/
    codas.emplace_back();
    auto start = get_phoneme(IPA::ɹ);
    auto candidates = std::views::all(phonemes) | std::views::filter(fricative) |
                      std::views::filter(except(IPA::h));
    for (const auto& c : candidates) {
      codas.back().emplace_back();
      codas.back().back().push_back(start);
      codas.back().back().push_back(&c);
    }
  }
  {  // Lateral approximant + nasal: /lm/, /ln/
    codas.emplace_back();
    auto start = get_phoneme(IPA::l);
    auto candidates = std::views::all(phonemes) | std::views::filter(nasal_c) |
                      std::views::filter(except(IPA::ŋ));
    for (const auto& c : candidates) {
      codas.back().emplace_back();
      codas.back().back().push_back(start);
      codas.back().back().push_back(&c);
    }
  }
  {  // In rhotic varieties, /r/ + nasal or lateral: /rm/, /rn/, /rl/
    codas.emplace_back();
    auto start = get_phoneme(IPA::ɹ);
    auto candidates = std::views::all(phonemes) | std::views::filter(nasal_c) |
                      std::views::filter(except(IPA::ŋ));
    for (const auto& c : candidates) {
      codas.back().emplace_back();
      codas.back().back().push_back(start);
      codas.back().back().push_back(&c);
    }
    codas.back().emplace_back();
    codas.back().back().push_back(start);
    codas.back().back().push_back(get_phoneme(IPA::l));
  }
  {  // Nasal + homorganic stop or affricate: /mp/, /nt/, /nd/, /ntʃ/, /ndʒ/,
     // /ŋk/
    codas.emplace_back();
    auto start = std::views::all(phonemes) | std::views::filter(nasal_c);
    auto candidates = std::views::all(phonemes) |
                      std::views::filter([](const auto& p) { return stop(p) || affricate(p); });
    for (const auto& s : start) {
      for (const auto& c : candidates) {
        if (!homorganic(&s.p, &c.p)) {
          continue;
        }
        codas.back().emplace_back();
        codas.back().back().push_back(&s);
        codas.back().back().push_back(&c);
      }
    }
  }
}

std::vector<const Phoneme*> AmericanEnglish::get_onset() const {
  std::vector<const Phoneme*> onset;
  int i = rand() % onsets.size();
  int j = rand() % onsets[i].size();
  for (auto p : onsets[i][j]) {
    onset.push_back(p);
  }
  return onset;
}

const Phoneme* AmericanEnglish::get_nucleus(const Phoneme* onset) const {
  if (auto it = nucleus_index_map.find(onset); it != nucleus_index_map.end()) {
    std::size_t i = it->second;
    std::size_t j = rand() % nuclei[i].size();
    return nuclei[i][j];
  }
  std::size_t i = rand() % nuclei.front().size();
  return nuclei.front()[i];
}

std::vector<const Phoneme*> AmericanEnglish::get_coda(const Phoneme* nucleus) const {
  if (rand() % 2 && !nuclei_requiring_coda.contains(nucleus)) {
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

std::string AmericanEnglish::get_spelling(const Syllable& syllable, bool word_final) const {
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
