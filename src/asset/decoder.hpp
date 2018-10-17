#ifndef MINIGAME_SRC_ASSET_DECODER_HPP_INCLUDED
#define MINIGAME_SRC_ASSET_DECODER_HPP_INCLUDED

#include "decoder_tga.hpp"

#include <assert.h>


template <typename AssetOutput>
struct DecodeVisitor : DecoderTGA<AssetOutput> {
  
  using DecoderTGA<AssetOutput>::operator();

  template <typename A, typename B>
  std::optional<AssetOutput> operator()(A&, B&)
  {
    assert(!"UNREACHABLE");
    return std::nullopt;
  }
};

#endif // MINIGAME_SRC_ASSET_DECODER_HPP_INCLUDED
