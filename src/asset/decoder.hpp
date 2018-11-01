#ifndef MINIGAME_SRC_ASSET_DECODER_HPP_INCLUDED
#define MINIGAME_SRC_ASSET_DECODER_HPP_INCLUDED

#include "../third_party/debug_assert.hpp"

#include "decoder_tga.hpp"
#include "decoder_vorbis.hpp"

#include <optional>
#include <assert.h>


template <typename AssetOutput>
struct DecodeVisitor :
  DecoderTGA<AssetOutput>,
  DecoderVorbis<AssetOutput> {
  
  using DecoderTGA<AssetOutput>::operator();
  using DecoderVorbis<AssetOutput>::operator();

  template <typename A, typename B>
  std::optional<AssetOutput> operator()(A&, B&)
  {
    DEBUG_UNREACHABLE(assert_handler{});
    return std::nullopt;
  }
};

#endif // MINIGAME_SRC_ASSET_DECODER_HPP_INCLUDED
