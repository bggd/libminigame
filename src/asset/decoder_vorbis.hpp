#ifndef MINIGAME_SRC_ASSET_DECODER_VORBIS_HPP_INCLUDED
#define MINIGAME_SRC_ASSET_DECODER_VORBIS_HPP_INCLUDED

#include "asset.hpp"
#include "../third_party/stb_vorbis.hpp"
#include "../third_party/debug_assert.hpp"

#include <optional>
#include <stdint.h>


template <typename AssetOutput>
struct DecoderVorbis {

  std::optional<AssetOutput> operator()(AssetAudioVorbis&, AssetLoadedFileData& file_data)
  {
    int channels, sample_rate;
    int16_t* data;
    int data_len = stb_vorbis_decode_memory(
        file_data.data,
        static_cast<int>(file_data.length),
        &channels, &sample_rate, &data
    );

    DEBUG_ASSERT(data_len, assert_handler{});
    DEBUG_ASSERT(sample_rate == 44100, assert_handler{});
    DEBUG_ASSERT(channels == 1 || channels == 2, assert_handler{});

    AssetAudio audio;
    if (channels == 1) {
      audio.format = AssetAudio::Format::MONO_SHORT16;
    }
    else if (channels == 2) {
      audio.format = AssetAudio::Format::STEREO_SHORT16;
    }

    audio.length = data_len;
    audio.data = data;
    return audio;
  }

};

#endif // MINIGAME_SRC_ASSET_DECODER_VORBIS_HPP_INCLUDED
