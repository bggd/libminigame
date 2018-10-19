#ifndef MINIGAME_SRC_ASSET_DECODER_VORBIS_HPP_INCLUDED
#define MINIGAME_SRC_ASSET_DECODER_VORBIS_HPP_INCLUDED

#include "asset.hpp"
#include "stb_vorbis.hpp"

#include <optional>


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
    if (data_len < 0) { return std::nullopt; }

    if (sample_rate != 44100) {
      delete[] data;
      return std::nullopt;
    }

    AssetAudio audio;
    if (channels == 1) {
      audio.format = AssetAudio::Format::MONO_SHORT16;
    }
    else if (channels == 2) {
      audio.format = AssetAudio::Format::STEREO_SHORT16;
    }
    else {
      delete[] data;
      return std::nullopt;
    }
    audio.length = data_len;
    audio.data = data;
    return audio;
  }

};

#endif // MINIGAME_SRC_ASSET_DECODER_VORBIS_HPP_INCLUDED
