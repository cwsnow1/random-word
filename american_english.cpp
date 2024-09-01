#include "american_english.hpp"

#include <cstdlib>
#include <functional>
#include <ranges>
#include <string>
#include <vector>

#include "phonology.hpp"

std::function<bool(const phonology::Phoneme&)> except(
    const std::vector<phonology::IPA>& exceptions) {
  return [exceptions](const auto& p) {
    return std::find(exceptions.begin(), exceptions.end(), p.p.symbol) ==
           exceptions.end();
  };
}

// clang-format off

AmericanEnglish::AmericanEnglish() {
  using namespace phonology;
  auto& phons = system.phonemes;
  {
    using enum IPA;
    phons.emplace_back(get_phone(æ),  Spelling({"a"}));
    phons.emplace_back(get_phone(ɑ),  Spelling({"o", "al", "au", "aw", /* "ough", "augh" */}));
    phons.emplace_back(get_phone(ɪ),  Spelling({"i", /*"y"*/}));
    phons.emplace_back(get_phone(ɛ),  Spelling({"e", "ea"}));
    phons.emplace_back(get_phone(ə),  Spelling({"a", "e", "o", "u", "ou"}));
    phons.emplace_back(get_phone(ʊ),  Spelling({"u", "oo", "o"}));
    phons.emplace_back(get_phone(eɪ), Spelling({"a", "ai", "ay"}));
    phons.emplace_back(get_phone(oʊ), Spelling({"o", "oa", "ow"}));
    phons.emplace_back(get_phone(i),  Spelling({"e", "ea", "ee", "y"}));
    phons.emplace_back(get_phone(u),  Spelling({"u", "oo", "ew"}));
    phons.emplace_back(get_phone(aɪ), Spelling({"i", "y", "igh"}));
    phons.emplace_back(get_phone(ɔɪ), Spelling({"oi", "oy"}));
    phons.emplace_back(get_phone(aʊ), Spelling({"ou", "ow"}));
    phons.emplace_back(get_phone(m),  Spelling({"m"}, {"m", "mm"}, {"m", "me"}));
    phons.emplace_back(get_phone(n),  Spelling({"n"}, {"n", "nn"}, {"n", "ne"}));
    phons.emplace_back(get_phone(ŋ),  Spelling({}, {"ng"}, {"ng"}));
    phons.emplace_back(get_phone(p),  Spelling({"p"}, {"p", "pp"}, {"p", "pe", /*"pp"*/}));
    phons.emplace_back(get_phone(t),  Spelling({"t"}, {"t", "tt"}, {"t", "te"}));
    phons.emplace_back(get_phone(tʃ), Spelling({"ch"}, {"ch", "tch"}, {"ch", "tch"}));
    phons.emplace_back(get_phone(k),  Spelling({"c", "k"}, {"c", "k", "ck"}, {"k", "ke", "ck"}));
    phons.emplace_back(get_phone(b),  Spelling({"b"}, {"b", "bb"}, {"b", "be"}));
    phons.emplace_back(get_phone(d),  Spelling({"d"}, {"d", "dd"}, {"d", "de"}));
    phons.emplace_back(get_phone(dʒ), Spelling({"j"}, {"j", "dge"}, {"ge", "dge"}));
    phons.emplace_back(get_phone(g),  Spelling({"g"}, {"g", "gg"}, {"g", "gg"}));
    phons.emplace_back(get_phone(f),  Spelling({"f", "ph"}, {"f", "f", "ph"}, {"f", "fe", "ph"}));
    phons.emplace_back(get_phone(θ),  Spelling({"th"}, {"th"}, {"th"}));
    phons.emplace_back(get_phone(s),  Spelling({"s"}, {"s", "ss"}, {"s", "ss", "ce"}));
    phons.emplace_back(get_phone(ʃ),  Spelling({"sh"}, {"sh"}, {"sh"}));
    phons.emplace_back(get_phone(v),  Spelling({"v"}, {"v"}, {"v", "ve"}));
    phons.emplace_back(get_phone(ð),  Spelling({"th"}, {"th"}, {"th", "the"}));
    phons.emplace_back(get_phone(z),  Spelling({"z"}, {"z", "s"}, {"z", "s"}));
    phons.emplace_back(get_phone(ʒ),  Spelling({"j"}, {"si", "s", "ge"}, {"ge"}));
    phons.emplace_back(get_phone(h),  Spelling({"h"}, {}, {}));
    phons.emplace_back(get_phone(w),  Spelling({"w"}, {}, {}));
    phons.emplace_back(get_phone(l),  Spelling({"l"}, {"l"}, {"ll", "l"}));
    phons.emplace_back(get_phone(r),  Spelling({"r"}, {"r"}, {"r"}));
    phons.emplace_back(get_phone(j),  Spelling({"y", "i"}, {}, {}));
  }

  using PFilter = std::function<bool(const Phoneme&)>;

  // Defining helper functions
  PFilter consonant   = [](const auto& p) { return !p.p.vowel; };
  PFilter vowel       = [](const auto& p) { return p.p.vowel; };
  PFilter stop        = [](const auto& p) { return p.p.moa == MoA::PLOSIVE; };
  PFilter approximant = [](const auto& p) { return p.p.moa == MoA::APPROXIMANT; };
  PFilter fricative   = [](const auto& p) { return p.p.moa == MoA::FRICATIVE; };
  PFilter affricate   = [](const auto& p) { return p.p.moa == MoA::AFFRICATE; };
  PFilter nasal       = [](const auto& p) { return p.p.moa == MoA::NASAL; };
  PFilter voiced      = [](const auto& p) { return p.p.voiced; };
  PFilter voiceless   = [](const auto& p) { return !p.p.voiced; };
  PFilter sibilant    = [](const auto& p) {
    return (p.p.moa == MoA::FRICATIVE) &&
           (p.p.poa == PoA::ALVEOLAR || p.p.poa == PoA::POST_ALVEOLAR);
  };
  // clang-format on

  // Constructing possible onsets, nucelei, and codas
  {
    // All single-consonant phonemes except /ŋ/
    system.onsets.emplace_back();
    auto candidates = std::views::all(phons) | std::views::filter(consonant) |
                      std::views::filter(except({IPA::ŋ}));
    for (const auto& c : candidates) {
      system.onsets.back().emplace_back();
      system.onsets.back().back().push_back(&c);
    }
  }

  {
    // Stop plus approximant other than /j/
    system.onsets.emplace_back();
    auto stops = std::views::all(phons) | std::views::filter(stop);
    auto approximants = std::views::all(phons) |
                        std::views::filter(approximant) |
                        std::views::filter(except({IPA::j}));
    for (const auto& s : stops) {
      for (const auto& a : approximants) {
        if (s.p.poa == a.p.poa) {
          continue;
        }
        system.onsets.back().emplace_back();
        system.onsets.back().back().push_back(&s);
        system.onsets.back().back().push_back(&a);
      }
    }
  }
  {
    // Voiceless fricative except /h/ plus approximant other than /j/
    // Exception /s/ + /r/ is not possible
    system.onsets.emplace_back();
    auto fricatives = std::views::all(phons) | std::views::filter(fricative) |
                      std::views::filter(voiceless) |
                      std::views::filter(except({IPA::h}));
    auto approximants = std::views::all(phons) |
                        std::views::filter(approximant) |
                        std::views::filter(except({IPA::j}));
    for (const auto& f : fricatives) {
      for (const auto& a : approximants) {
        if (f.p.poa == a.p.poa ||
            (f.p.symbol == IPA::s && a.p.symbol == IPA::r)) {
          continue;
        }
        system.onsets.back().emplace_back();
        system.onsets.back().back().push_back(&f);
        system.onsets.back().back().push_back(&a);
      }
    }
  }
  {
    // /s/ plus voiceless stop
    system.onsets.emplace_back();
    auto s = system.get_phoneme(IPA::s);
    auto stops = std::views::all(phons) | std::views::filter(stop) |
                 std::views::filter(voiceless);
    for (const auto& plosive : stops) {
      system.onsets.back().emplace_back();
      system.onsets.back().back().push_back(s);
      system.onsets.back().back().push_back(&plosive);
    }
  }
  {
    // /s/ plus nasal other than /ŋ/
    system.onsets.emplace_back();
    auto s = system.get_phoneme(IPA::s);
    auto nasals = std::views::all(phons) | std::views::filter(nasal) |
                  std::views::filter(except({IPA::ŋ}));
    for (const auto& n : nasals) {
      system.onsets.back().emplace_back();
      system.onsets.back().back().push_back(s);
      system.onsets.back().back().push_back(&n);
    }
  }
  /* I don't believe this rule
  {
    // /s/ plus voiceless non-sibilant fricative
    system.onsets.emplace_back();
    auto s = system.get_phoneme(IPA::s);
    auto fricatives =
        std::views::all(phons) | std::views::filter(fricative) |
        std::views::filter(voiceless) |
        std::views::filter([sibilant](const auto& p) { return !sibilant(p); });
    for (const auto& f : fricatives) {
      system.onsets.back().emplace_back();
      system.onsets.back().back().push_back(s);
      system.onsets.back().back().push_back(&f);
    }
  }
  */
  {
    // /s/ plus voiceless stop plus approximant except /r/
    system.onsets.emplace_back();
    auto s = system.get_phoneme(IPA::s);
    auto stops = std::views::all(phons) | std::views::filter(voiceless) |
                 std::views::filter(stop);
    auto approximants = std::views::all(phons) |
                        std::views::filter(approximant) |
                        std::views::filter(except({IPA::r}));
    for (const auto& plosive : stops) {
      for (const auto& a : approximants) {
        if (plosive.p.poa == a.p.poa) continue;
        system.onsets.back().emplace_back();
        system.onsets.back().back().push_back(s);
        system.onsets.back().back().push_back(&plosive);
        system.onsets.back().back().push_back(&a);
      }
    }
  }
  //
  // Nuclei
  //
  {
    system.nuclei.emplace_back();
    auto candidates = std::views::all(phons) | std::views::filter(vowel);
    for (const auto& c : candidates) {
      system.nuclei.back().push_back(&c);
    }
  }
  //
  // Codas
  //
  {
    // The single consonant phonemes except /h/, /w/, /j/
    system.codas.emplace_back();
    auto candidates = std::views::all(phons) | std::views::filter(consonant) |
                      std::views::filter(except({IPA::h, IPA::w, IPA::j}));
    for (const auto& c : candidates) {
      system.codas.back().emplace_back();
      system.codas.back().back().push_back(&c);
    }
  }
  {
    // Lateral approximant plus stop or affricate: /lp/,
    // /lb/, /lt/, /ld/, /ltʃ/, /ldʒ/, /lk/
    system.codas.emplace_back();
    auto l = system.get_phoneme(IPA::l);
    auto candidates = std::views::all(phons) |
                      std::views::filter([stop, affricate](const auto& p) {
                        return stop(p) || affricate(p);
                      });
    for (const auto& c : candidates) {
      system.codas.back().emplace_back();
      system.codas.back().back().push_back(l);
      system.codas.back().back().push_back(&c);
    }
  }
  {
    // In rhotic varieties, /r/ plus stop or affricate: /rp/,
    // /rb/, /rt/, /rd/, /rtʃ/, /rdʒ/, /rk/, /rɡ/
    system.codas.emplace_back();
    auto r = system.get_phoneme(IPA::r);
    auto candidates = std::views::all(phons) |
                      std::views::filter([stop, affricate](const auto& p) {
                        return stop(p) || affricate(p);
                      });
    for (const auto& c : candidates) {
      system.codas.back().emplace_back();
      system.codas.back().back().push_back(r);
      system.codas.back().back().push_back(&c);
    }
  }
  {
    // Lateral approximant + fricative except /h/: /lf/, /lv/,
    // /lθ/, /ls/, /lz/, /lʃ/, (/lð/)
    system.codas.emplace_back();
    auto l = system.get_phoneme(IPA::l);
    auto candidates = std::views::all(phons) | std::views::filter(fricative) |
                      std::views::filter(except({IPA::h}));
    for (const auto& c : candidates) {
      system.codas.back().emplace_back();
      system.codas.back().back().push_back(l);
      system.codas.back().back().push_back(&c);
    }
  }
}
