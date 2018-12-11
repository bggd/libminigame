#ifndef MINIGAME_INCLUDE_MINIGAME_MINIGAME_H_INCLUDED
#define MINIGAME_INCLUDE_MINIGAME_MINIGAME_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
    #define MINIGAME_EXPORT __declspec(dllexport)
#else
    #define MINIGAME_EXPORT
#endif

enum MINIGAME_ASSET_TYPE {
  MINIGAME_ASSET_TYPE_IMAGE_TGA,
  MINIGAME_ASSET_TYPE_AUDIO_VORBIS,
};


MINIGAME_EXPORT extern void minigame_init();
MINIGAME_EXPORT extern void minigame_deinit();

MINIGAME_EXPORT extern void minigame_asset_load(const char* path, enum MINIGAME_ASSET_TYPE type);

MINIGAME_EXPORT extern void minigame_window_open();
MINIGAME_EXPORT extern void minigame_window_swap_buffers();
MINIGAME_EXPORT extern void minigame_window_close();

#ifdef __cplusplus
}
#endif


#endif // MINIGAME_INCLUDE_MINIGAME_MINIGAME_H_INCLUDED
