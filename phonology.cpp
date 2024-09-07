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
    {IPA::a,  Phone(IPA::a,  VR::UNROUNDED, VH::OPEN,         VB::FRONT,   VN::ORAL)},
    {IPA::aɪ, Phone(IPA::aɪ, VR::UNROUNDED, VH::OPEN,         VB::FRONT,   VN::ORAL)},
    {IPA::aʊ, Phone(IPA::aʊ, VR::UNROUNDED, VH::OPEN,         VB::FRONT,   VN::ORAL)},
    {IPA::æ,  Phone(IPA::æ,  VR::UNROUNDED, VH::NEAR_OPEN,    VB::FRONT,   VN::ORAL)},
    {IPA::ɛ,  Phone(IPA::ɛ,  VR::UNROUNDED, VH::OPEN_MID,     VB::FRONT,   VN::ORAL)},
    {IPA::ɛ̃,  Phone(IPA::ɛ̃,  VR::UNROUNDED, VH::OPEN_MID,     VB::FRONT,   VN::NASAL)},
    {IPA::œ,  Phone(IPA::œ,  VR::ROUNDED,   VH::OPEN_MID,     VB::FRONT,   VN::ORAL)},
    {IPA::e,  Phone(IPA::e,  VR::UNROUNDED, VH::CLOSE_MID,    VB::FRONT,   VN::ORAL)},
    {IPA::eɪ, Phone(IPA::eɪ, VR::UNROUNDED, VH::CLOSE_MID,    VB::FRONT,   VN::ORAL)},
    {IPA::ø,  Phone(IPA::ø,  VR::ROUNDED,   VH::CLOSE_MID,    VB::FRONT,   VN::ORAL)},
    {IPA::ɪ,  Phone(IPA::ɪ,  VR::UNROUNDED, VH::NEAR_CLOSE,   VB::FRONT,   VN::ORAL)},
    {IPA::i,  Phone(IPA::i,  VR::UNROUNDED, VH::CLOSE,        VB::FRONT,   VN::ORAL)},
    {IPA::y,  Phone(IPA::y,  VR::ROUNDED,   VH::CLOSE,        VB::FRONT,   VN::ORAL)},
    {IPA::ɑ,  Phone(IPA::ɑ,  VR::UNROUNDED, VH::OPEN,         VB::BACK,    VN::ORAL)},
    {IPA::ɑ̃,  Phone(IPA::ɑ̃,  VR::UNROUNDED, VH::OPEN,         VB::BACK,    VN::NASAL)},
    {IPA::ɔ,  Phone(IPA::ɔ,  VR::ROUNDED,   VH::OPEN_MID,     VB::BACK,    VN::ORAL)},
    {IPA::ɔ̃,  Phone(IPA::ɔ̃,  VR::ROUNDED,   VH::OPEN_MID,     VB::BACK,    VN::NASAL)},
    {IPA::ɔɪ, Phone(IPA::ɔɪ, VR::ROUNDED,   VH::OPEN_MID,     VB::BACK,    VN::ORAL)},
    {IPA::o,  Phone(IPA::o,  VR::ROUNDED,   VH::CLOSE_MID,    VB::BACK,    VN::ORAL)},
    {IPA::oʊ, Phone(IPA::oʊ, VR::ROUNDED,   VH::CLOSE_MID,    VB::BACK,    VN::ORAL)},
    {IPA::ʊ,  Phone(IPA::ʊ,  VR::ROUNDED,   VH::NEAR_CLOSE,   VB::BACK,    VN::ORAL)},
    {IPA::ə,  Phone(IPA::ə,  VR::UNROUNDED, VH::MID,          VB::CENTRAL, VN::ORAL)},
    {IPA::u,  Phone(IPA::u,  VR::ROUNDED,   VH::CLOSE,        VB::BACK,    VN::ORAL)},
    {IPA::m,  Phone(IPA::m,  CV::VOICED,    MoA::NASAL,       PoA::LABIAL)},
    {IPA::n,  Phone(IPA::n,  CV::VOICED,    MoA::NASAL,       PoA::ALVEOLAR)},
    {IPA::ɲ,  Phone(IPA::ɲ,  CV::VOICED,    MoA::NASAL,       PoA::PALATAL)},
    {IPA::ŋ,  Phone(IPA::ŋ,  CV::VOICED,    MoA::NASAL,       PoA::VELAR)},
    {IPA::p,  Phone(IPA::p,  CV::VOICELESS, MoA::PLOSIVE,     PoA::LABIAL)},
    {IPA::t,  Phone(IPA::t,  CV::VOICELESS, MoA::PLOSIVE,     PoA::ALVEOLAR)},
    {IPA::tʃ, Phone(IPA::tʃ, CV::VOICELESS, MoA::AFFRICATE,   PoA::POST_ALVEOLAR)},
    {IPA::k,  Phone(IPA::k,  CV::VOICELESS, MoA::PLOSIVE,     PoA::VELAR)},
    {IPA::b,  Phone(IPA::b,  CV::VOICED,    MoA::PLOSIVE,     PoA::LABIAL)},
    {IPA::d,  Phone(IPA::d,  CV::VOICED,    MoA::PLOSIVE,     PoA::ALVEOLAR)},
    {IPA::dʒ, Phone(IPA::dʒ, CV::VOICED,    MoA::AFFRICATE,   PoA::POST_ALVEOLAR)},
    {IPA::g,  Phone(IPA::g,  CV::VOICED,    MoA::PLOSIVE,     PoA::VELAR)},
    {IPA::f,  Phone(IPA::f,  CV::VOICELESS, MoA::FRICATIVE,   PoA::LABIAL)},
    {IPA::θ,  Phone(IPA::θ,  CV::VOICELESS, MoA::FRICATIVE,   PoA::DENTAL)},
    {IPA::s,  Phone(IPA::s,  CV::VOICELESS, MoA::FRICATIVE,   PoA::ALVEOLAR)},
    {IPA::ʃ,  Phone(IPA::ʃ,  CV::VOICELESS, MoA::FRICATIVE,   PoA::POST_ALVEOLAR)},
    {IPA::h,  Phone(IPA::h,  CV::VOICELESS, MoA::FRICATIVE,   PoA::GLOTTAL)},
    {IPA::v,  Phone(IPA::v,  CV::VOICED,    MoA::FRICATIVE,   PoA::LABIAL)},
    {IPA::ð,  Phone(IPA::ð,  CV::VOICED,    MoA::FRICATIVE,   PoA::DENTAL)},
    {IPA::z,  Phone(IPA::z,  CV::VOICED,    MoA::FRICATIVE,   PoA::ALVEOLAR)},
    {IPA::ʒ,  Phone(IPA::ʒ,  CV::VOICED,    MoA::FRICATIVE,   PoA::POST_ALVEOLAR)},
    {IPA::w,  Phone(IPA::w,  CV::VOICED,    MoA::APPROXIMANT, PoA::LABIAL)},
    {IPA::l,  Phone(IPA::l,  CV::VOICED,    MoA::APPROXIMANT, PoA::ALVEOLAR)},
    {IPA::ɹ,  Phone(IPA::ɹ,  CV::VOICED,    MoA::APPROXIMANT, PoA::POST_ALVEOLAR)},
    {IPA::ɥ,  Phone(IPA::ɥ,  CV::VOICED,    MoA::APPROXIMANT, PoA::PALATAL)},
    {IPA::ʁ̞,  Phone(IPA::ʁ̞,  CV::VOICED,    MoA::APPROXIMANT, PoA::UVULAR)},
    {IPA::j,  Phone(IPA::j,  CV::VOICED,    MoA::APPROXIMANT, PoA::PALATAL)},
    // clang-format on
};

// Private functions

// Public functions

bool homorganic(const Phone* lhs, const Phone* rhs) {
  if (lhs->poa == rhs->poa) {
    return true;
  }
  return (lhs->poa == PoA::ALVEOLAR && rhs->poa == PoA::POST_ALVEOLAR) ||
         (lhs->poa == PoA::POST_ALVEOLAR && rhs->poa == PoA::ALVEOLAR);
}

Phone get_phone(IPA symbol) { return phones.at(symbol); }

}  // namespace phonology
