#include "../include/minigame/minigame.h"

#include "asset/asset_loader.hpp"
#include "audio/audio_player.hpp"
#include "window/window.hpp"
#include "third_party/glfw.hpp"
#include "third_party/opengl.hpp"
#include "third_party/debug_assert.hpp"

#include <thread>
#include <stdlib.h>


static bool g_is_initialized = false;

static AssetLoader g_asset_loader;
static AudioPlayer g_audio_player;

static std::thread g_thread_load;
static std::thread g_thread_decode;
static std::thread g_thread_audio_update;

static ALCdevice* g_al_device;
static ALCcontext* g_al_ctx;

static Window g_window;

static void init_threads() noexcept
{
  try {
    g_thread_load = std::thread(&AssetLoader::thread_for_load_file, &g_asset_loader);
    g_thread_decode = std::thread(&AssetLoader::thread_for_decode_buffer, &g_asset_loader);
    g_thread_audio_update = std::thread(&AudioPlayer::thread_for_update, &g_audio_player);
  }
  catch (std::exception& e) {
    DEBUG_UNREACHABLE(assert_handler{}, e.what());
    abort();
  }
}

static void deinit_threads() noexcept
{
  g_asset_loader.queue_load.close();
  g_asset_loader.queue_decode.close();
  g_audio_player.is_close = true;

  try {
    g_thread_load.join();
    g_thread_decode.join();
    g_thread_audio_update.join();
  }
  catch (std::exception& e) {
    DEBUG_UNREACHABLE(assert_handler{}, e.what());
    abort();
  }
}

static void init_openal() noexcept
{
  g_al_device = alcOpenDevice(nullptr);
  DEBUG_ASSERT(g_al_device, assert_handler{});

  g_al_ctx = alcCreateContext(g_al_device, nullptr);
  DEBUG_ASSERT(g_al_ctx, assert_handler{});

  alcMakeContextCurrent(g_al_ctx);

  alGetError();
}

static void deinit_openal() noexcept
{
  alcMakeContextCurrent(nullptr);
  alcDestroyContext(g_al_ctx);
  alcCloseDevice(g_al_device);
}

static void error_callback(int, const char* description)
{
  DEBUG_UNREACHABLE(assert_handler{}, description);
}

static void init_glfw() noexcept
{
  glfwSetErrorCallback(error_callback);

  int err = glfwInit();
  DEBUG_ASSERT(err == GLFW_TRUE, assert_handler{});
}

static void deinit_glfw() noexcept
{
  glfwTerminate();
}

extern "C" {

void minigame_init()
{
  DEBUG_ASSERT(g_is_initialized == false, assert_handler{});

  init_threads();
  init_openal();
  init_glfw();

  g_is_initialized = true;
}

void minigame_deinit()
{
  DEBUG_ASSERT(g_is_initialized, assert_handler{});

  deinit_glfw();
  deinit_openal();
  deinit_threads();
}

void minigame_asset_load(const char* path, enum MINIGAME_ASSET_TYPE type)
{
  if (type == MINIGAME_ASSET_TYPE_IMAGE_TGA) { g_asset_loader.load(path, AssetImageTGA{}); }
  else if (type == MINIGAME_ASSET_TYPE_AUDIO_VORBIS) { g_asset_loader.load(path, AssetAudioVorbis{}); }
}

void minigame_window_open()
{
  g_window.open();
  bool ok = load_gl();
  DEBUG_ASSERT(ok, assert_handler{});
}

void minigame_window_swap_buffers()
{
  g_window.swap_buffers();
}

void minigame_window_close()
{
  g_window.close();
}

}
