#ifndef MINIGAME_TEST_AUDIO_TEST_AUDIO_PLAYER_HPP_INCLUDED
#define MINIGAME_TEST_AUDIO_TEST_AUDIO_PLAYER_HPP_INCLUDED

#include <boost/core/lightweight_test.hpp>

#include "../../src/audio/audio_player.hpp"

#include <thread>


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

  AssetLoader<asset_t, asset_specific_t, DecodeVisitor<asset_t>> al;
  al.load("Upbeat Loop.ogg", AssetAudioVorbis{});

  std::thread th_load(&decltype(al)::thread_for_load_file, &al);
  std::thread th_decode(&decltype(al)::thread_for_decode_buffer, &al);

  while (!al.is_complete()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  {
    AudioPlayer player;

    std::thread th_cmd(&decltype(player)::thread_for_command, &player);
    std::thread th_stream(&decltype(player)::thread_for_stream, &player);

    auto i = player.play(al.get("Upbeat Loop.ogg").value(), true);
    i->push_pause_cmd();
    i->push_play_cmd();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    player.queue.close();
    player.is_close = true;

    th_cmd.join();
    th_stream.join();
  }

  deinit_openal();

  al.queue_load.close();
  al.queue_decode.close();

  th_load.join();
  th_decode.join();

}

#endif // MINIGAME_TEST_AUDIO_TEST_AUDIO_PLAYER_HPP_INCLUDED
