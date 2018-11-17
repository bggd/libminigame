#ifndef MINIGAME_TEST_AUDIO_TEST_AUDIO_PLAYER_HPP_INCLUDED
#define MINIGAME_TEST_AUDIO_TEST_AUDIO_PLAYER_HPP_INCLUDED

#include <boost/core/lightweight_test.hpp>

#include "../../src/audio/audio_player.hpp"

#include <thread>


static_assert(std::is_nothrow_copy_constructible<std::shared_ptr<AudioInstance>>::value);
static_assert(std::is_nothrow_copy_assignable<std::shared_ptr<AudioInstance>>::value);

ALCdevice* g_al_device;
ALCcontext* g_al_ctx;


bool init_openal()
{
  g_al_device = alcOpenDevice(nullptr);
  if (!g_al_device) { return false; }

  g_al_ctx = alcCreateContext(g_al_device, nullptr);
  if (!g_al_ctx) { return false; }

  alcMakeContextCurrent(g_al_ctx);

  alGetError();

  return true;
}

void deinit_openal()
{
  alcMakeContextCurrent(nullptr);
  alcDestroyContext(g_al_ctx);
  alcCloseDevice(g_al_device);
}

void test_audio_player()
{
  BOOST_TEST(init_openal());

  AssetLoader al;
  al.load("Upbeat Loop.ogg", AssetAudioVorbis{});

  std::thread th_load(&decltype(al)::thread_for_load_file, &al);
  std::thread th_decode(&decltype(al)::thread_for_decode_buffer, &al);

  while (!al.is_complete()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  {
    std::shared_ptr<AudioPlayer> player = std::make_shared<AudioPlayer>();

    std::thread th_update(&AudioPlayer::thread_for_update, player.get());

    auto ai = player->play(al.get("Upbeat Loop.ogg").value());

    ai->play();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    ai->pause();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    ai->rewind();
    ai->play();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    player->is_close = true;

    th_update.join();
  }

  deinit_openal();

  al.queue_load.close();
  al.queue_decode.close();

  th_load.join();
  th_decode.join();

}

#endif // MINIGAME_TEST_AUDIO_TEST_AUDIO_PLAYER_HPP_INCLUDED
