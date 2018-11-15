#ifndef MINIGAME_SRC_ASSET_ASSET_VORBIS_HPP_INCLUDED
#define MINIGAME_SRC_ASSET_ASSET_VORBIS_HPP_INCLUDED

#include "../third_party/stb_vorbis.hpp"
#include "../third_party/debug_assert.hpp"

#include <limits>
#include <stdlib.h>


struct AssetAudioVorbis : AssetAudio {

  bool is_static = true;
  size_t length = 0;
  int16_t* raw_samples = nullptr;
  stb_vorbis* vorbis = nullptr;
  uint8_t* data_for_vorbis = nullptr;

  void load_from_memory(uint8_t* file_data, size_t file_length) noexcept final
  {
    DEBUG_ASSERT(file_length <= std::numeric_limits<int>::max(), assert_handler{});

    if (this->is_static) {
      int channels, sample_rate;

      int data_len = stb_vorbis_decode_memory(
          file_data,
          static_cast<int>(file_length),
          &channels, &sample_rate, &this->raw_samples
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

      delete[] file_data;
    }
    else {
      int err;
      this->vorbis = stb_vorbis_open_memory(file_data, static_cast<int>(file_length), &err, nullptr);

      DEBUG_ASSERT(this->vorbis, assert_handler{});
      DEBUG_ASSERT(err == VORBIS__no_error, assert_handler{});

      stb_vorbis_info info = stb_vorbis_get_info(this->vorbis);

      DEBUG_ASSERT(info.sample_rate == 44100, assert_handler{});
      DEBUG_ASSERT(info.channels == 1 || info.channels == 2, assert_handler{});

      if (info.channels == 1) {
        this->format = AssetAudio::Format::MONO_SHORT16;
      }
      else if (info.channels == 2) {
        this->format = AssetAudio::Format::STEREO_SHORT16;
      }

      this->data_for_vorbis = file_data;
    }
  }

  void unload() noexcept final
  {
    if (this->is_static) {
      DEBUG_ASSERT(this->raw_samples, assert_handler{});

      free(this->raw_samples);
    }
    else {
      DEBUG_ASSERT(this->vorbis, assert_handler{});
      DEBUG_ASSERT(this->data_for_vorbis, assert_handler{});

      stb_vorbis_close(this->vorbis);

      delete [] this->data_for_vorbis;
    }
  }

};

#endif // MINIGAME_SRC_ASSET_ASSET_VORBIS_HPP_INCLUDED
