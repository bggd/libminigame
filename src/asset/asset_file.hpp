#ifndef MINIGAME_SRC_ASSET_ASSET_FILE_HPP_INCLUDED
#define MINIGAME_SRC_ASSET_ASSET_FILE_HPP_INCLUDED

struct AssetFile : AssetBase {

  const uint8_t* data = nullptr;
  size_t size = 0;

  void load_from_memory(uint8_t* file_data, size_t file_length) noexcept override
  {
    this->data = file_data;
    this->size = file_length;
  }

  void unload() noexcept override
  {
    delete[] this->data;
  }

};

#endif // MINIGAME_SRC_ASSET_ASSET_FILE_HPP_INCLUDED
