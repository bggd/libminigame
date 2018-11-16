#ifndef MINIGAME_SRC_AUDIO_AUDIO_PLAYER_HPP_INCLUDED
#define MINIGAME_SRC_AUDIO_AUDIO_PLAYER_HPP_INCLUDED

#include "../asset/asset.hpp"
#include "../thread/thread_queue.hpp"
#include "../third_party/openal.hpp"
#include "../third_party/debug_assert.hpp"

#include <memory>
#include <list>
#include <thread>
#include <chrono>
#include <optional>
#include <array>
#include <utility>
#include <stdio.h>
#include <stdlib.h>

#define MINIGAME_AUDIO_BUFFER_SIZE 4096
#define MINIGAME_AUDIO_QUEUE_SIZE 4

#define AL_CHECK(stmt) \
  do { \
    stmt; \
    openal_check_error(#stmt, __FILE__, __LINE__); \
  } while (0);


const char* openal_err2str(ALenum err) noexcept;
void openal_check_error(const char* stmt, const char* file, unsigned int line) noexcept;

struct AudioInstance : std::enable_shared_from_this<AudioInstance> {

  enum class State {
    PLAY,
    PAUSE
  };

  ALuint source;
  std::array<ALuint, MINIGAME_AUDIO_QUEUE_SIZE> buffers;
  std::weak_ptr<AssetBase> asset;
  ALsizei readed_samples = 0;
  bool is_initialized = false;
  bool is_loop = false;
  AudioInstance::State state;
  std::atomic<bool> is_playing = false;
  std::atomic<bool> request_rewind = false;

  ~AudioInstance() { this->deinit(); }
  void init() noexcept;
  void deinit() noexcept;
  void unqueue_buffers() noexcept;
  void fill_queue(ALuint) noexcept;
  void fill_queue_before_play() noexcept;
  void update() noexcept;
  void play() noexcept;
  void pause() noexcept;
  void rewind() noexcept;

};


struct AudioPlayer : std::enable_shared_from_this<AudioPlayer> {

  std::list<std::shared_ptr<AudioInstance>> instances;
  ThreadQueue<std::shared_ptr<AudioInstance>> queue;
  std::atomic<bool> is_close = false;

  void thread_for_update() noexcept;

  std::shared_ptr<AudioInstance> play(std::weak_ptr<AssetBase> asset, bool loop=false) noexcept;

};


const char* openal_err2str(ALenum err) noexcept
{
  switch (err) {
    case AL_NO_ERROR: return "AL_NO_ERROR";
    case AL_INVALID_NAME: return "AL_INVALID_NAME";
    case AL_INVALID_VALUE: return "AL_INVALID_VALUE";
    case AL_INVALID_OPERATION: return "AL_INVALID_OPERATION";
    case AL_OUT_OF_MEMORY: return "AL_OUT_OF_MEMORY";
    default: DEBUG_UNREACHABLE(assert_handler{});
  }
  return "";
}

void openal_check_error(const char* stmt, const char* file, unsigned int line) noexcept
{
  struct al_assert_handler : debug_assert::default_handler, debug_assert::set_level<-1> {

    static void handle(const debug_assert::source_location&, const char*, const char* err_msg, const char* stmt, const char* file, unsigned int line)
    {
      debug_assert::source_location loc;
      loc.file_name = file;
      loc.line_number = line;

      try {
        std::string str = std::string(stmt) + " " + std::string(err_msg);

        debug_assert::default_handler::handle(loc, str.data());
      }
      catch (const std::exception& e) {
        DEBUG_UNREACHABLE(assert_handler{}, e.what());
        abort();
      }
    }

  };

  ALenum err;
  err = alGetError();
  DEBUG_ASSERT(err == AL_NO_ERROR, al_assert_handler{}, openal_err2str(err), stmt, file, line);
}

void AudioInstance::init() noexcept
{
  DEBUG_ASSERT(!this->is_initialized, assert_handler{});

  AL_CHECK(alGenSources(1, &this->source));

  AL_CHECK(alGenBuffers(MINIGAME_AUDIO_QUEUE_SIZE, this->buffers.data()));

  this->is_initialized = true;
}

void AudioInstance::deinit() noexcept
{
  DEBUG_ASSERT(this->is_initialized, assert_handler{});

  this->unqueue_buffers();

  AL_CHECK(alDeleteBuffers(MINIGAME_AUDIO_QUEUE_SIZE, this->buffers.data()));

  AL_CHECK(alDeleteSources(1, &this->source));
}

void AudioInstance::unqueue_buffers() noexcept
{
  AL_CHECK(alSourcef(this->source, AL_GAIN, 0.0f));
  AL_CHECK(alSourcePlay(this->source));
  AL_CHECK(alSourceStop(this->source));
  AL_CHECK(alSourcef(this->source, AL_GAIN, 1.0f));

  this->state = AudioInstance::State::PAUSE;

  ALint num_queue;
  AL_CHECK(alGetSourcei(this->source, AL_BUFFERS_QUEUED, &num_queue));

  ALuint unqueue_buffers[MINIGAME_AUDIO_QUEUE_SIZE];
  AL_CHECK(alSourceUnqueueBuffers(this->source, num_queue, unqueue_buffers));
}

void AudioInstance::fill_queue(ALuint id) noexcept
{
  std::shared_ptr<AssetBase> base = this->asset.lock();
  DEBUG_ASSERT(base, assert_handler{});

  std::shared_ptr<AssetAudio> audio = std::static_pointer_cast<AssetAudio>(base);
  DEBUG_ASSERT(audio, assert_handler{});

  ALenum format = (audio->format == AssetAudio::Format::MONO_SHORT16) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

  if (audio->is_static) {
    if (this->readed_samples == audio->num_samples) {
      if (this->is_loop) { this->readed_samples = 0; }
      else { return; }
    }

    ALsizei n = MINIGAME_AUDIO_BUFFER_SIZE / 2;

    if (this->readed_samples + n > audio->num_samples) {
      n = audio->num_samples - this->readed_samples;
    }

    ALsizei size = sizeof(int16_t) * n;

    AL_CHECK(alBufferData(id, format, &audio->raw_samples[this->readed_samples], size, 44100));

    AL_CHECK(alSourceQueueBuffers(this->source, 1, &id));

    this->readed_samples += n;
  }
  else {
    const int num_elem = MINIGAME_AUDIO_BUFFER_SIZE / 2;
    int16_t data[num_elem];

    size_t n;
    n = audio->decode(data, num_elem);

    if (n < num_elem && this->is_loop) {
      audio->rewind();
      n = audio->decode(data+n, num_elem-n);
    }

    if (n == 0) { return; }

    ALsizei size = sizeof(int16_t) * (ALsizei)n;

    AL_CHECK(alBufferData(id, format, data, size, 44100));

    AL_CHECK(alSourceQueueBuffers(this->source, 1, &id));
  }
}

void AudioInstance::fill_queue_before_play() noexcept
{
  for (int i = 0; i < MINIGAME_AUDIO_QUEUE_SIZE; ++i) {
    this->fill_queue(this->buffers[i]);
  }
}

void AudioInstance::update() noexcept
{
  if (this->request_rewind) {
    this->unqueue_buffers();

    std::shared_ptr<AssetBase> base = this->asset.lock();
    DEBUG_ASSERT(base, assert_handler{});

    std::shared_ptr<AssetAudio> audio = std::static_pointer_cast<AssetAudio>(base);
    DEBUG_ASSERT(audio, assert_handler{});

    if (audio->is_static) { this->readed_samples = 0;}
    else { audio->rewind(); }

    this->fill_queue_before_play();
    this->request_rewind = false;
  }

  if (this->is_playing && this->state == AudioInstance::State::PAUSE) {
    AL_CHECK(alSourcePlay(this->source));
    this->state = AudioInstance::State::PLAY;
  }
  else if (this->is_playing == false) {
    AL_CHECK(alSourcePause(this->source));
    this->state = AudioInstance::State::PAUSE;
  }

  ALint num_processed;
  AL_CHECK(alGetSourcei(this->source, AL_BUFFERS_PROCESSED, &num_processed));

  for (int i = 0; i < num_processed; ++i) {
    ALuint id;
    AL_CHECK(alSourceUnqueueBuffers(this->source, 1, &id));
    this->fill_queue(id);
  }
}

void AudioInstance::play() noexcept
{
  this->is_playing = true;
}

void AudioInstance::pause() noexcept
{
  this->is_playing = false;
}

void AudioInstance::rewind() noexcept
{
  this->request_rewind = true;
}

void AudioPlayer::thread_for_update() noexcept
{
  while (!this->is_close) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    while (!this->queue.is_empty()) {
      std::optional<std::shared_ptr<AudioInstance>> opt = this->queue.pop();

      DEBUG_ASSERT(opt, assert_handler{});

      std::shared_ptr<AudioInstance> ai = *opt;

      this->instances.push_back(ai);
    }

    for (std::shared_ptr<AudioInstance> ai : this->instances) {
      if (!ai->is_initialized) { ai->init(); ai->fill_queue_before_play(); }
      ai->update();
    }
  }
}

std::shared_ptr<AudioInstance> AudioPlayer::play(std::weak_ptr<AssetBase> asset, bool loop) noexcept
{
  std::shared_ptr<AudioInstance> ai;

  ai = std::make_shared<AudioInstance>();

  DEBUG_ASSERT(asset.lock(), assert_handler{});
  DEBUG_ASSERT(std::dynamic_pointer_cast<AssetAudio>(asset.lock()), assert_handler{});

  ai->asset = asset;
  ai->is_loop = loop;
  ai->state = AudioInstance::State::PAUSE;
  ai->is_playing = true;

  this->queue.push(ai);

  return ai;
}

#endif // MINIGAME_SRC_AUDIO_AUDIO_PLAYER_HPP_INCLUDED
