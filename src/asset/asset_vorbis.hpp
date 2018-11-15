#ifndef MINIGAME_SRC_ASSET_ASSET_VORBIS_HPP_INCLUDED
#define MINIGAME_SRC_ASSET_ASSET_VORBIS_HPP_INCLUDED

#include "../third_party/stb_vorbis.hpp"
#include "../third_party/debug_assert.hpp"


struct AssetAudioVorbis  : AssetAudio {

  void load_from_memory(uint8_t* file_data, size_t file_length) noexcept final
  {
    int channels, sample_rate;

    int data_len = stb_vorbis_decode_memory(
        file_data,
        static_cast<int>(file_length),
        &channels, &sample_rate, &this->data
    );

    DEBUG_ASSERT(data_len, assert_handler{});
    DEBUG_ASSERT(sample_rate == 44100, assert_handler{});
    DEBUG_ASSERT(channels == 1 || channels == 2, assert_handler{});

    if (channels == 1) {
      this->format = AssetAudio::Format::MONO_SHORT16;
    }
    else if (channels == 2) {
      this->format = AssetAudio::Format::STEREO_SHORT16;
    }

    this->length = data_len;
  }

};

#endif // MINIGAME_SRC_ASSET_ASSET_VORBIS_HPP_INCLUDED
