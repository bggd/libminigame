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

#define MINIGAME_AUDIO_BUFFER_SIZE 4096
#define MINIGAME_AUDIO_QUEUE_SIZE 4

#define AL_CHECK(stmt) \
  do { \
    stmt; \
    openal_check_error(#stmt, __FILE__, __LINE__); \
  } while (0);


const char* openal_err2str(ALenum err);
void openal_check_error(const char* stmt, const char* file, unsigned int line);


struct AudioInstance {

  ALuint source;
  std::array<ALuint, MINIGAME_AUDIO_QUEUE_SIZE> buffers;
  std::weak_ptr<asset_t> asset;
  ALsizei readed_bytes = 0;
  bool is_end_of_stream = false;

  ~AudioInstance() { deinit(); printf("dtor\n"); }

  void init();
  void deinit();
  void fill_queue_before_play();
  void play();
  void update();

};


struct AudioPlayer {

  std::list<std::shared_ptr<AudioInstance>> instances;
  ThreadQueue<std::shared_ptr<AudioInstance>> queue;
  std::mutex mtx;
  std::atomic<bool> is_close = false;

  void thread_for_init();
  void thread_for_stream();

  std::shared_ptr<AudioInstance> play(std::weak_ptr<asset_t> asset);

};


const char* openal_err2str(ALenum err)
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

void openal_check_error(const char* stmt, const char* file, unsigned int line)
{
  struct al_assert_handler : debug_assert::default_handler, debug_assert::set_level<-1> {

    static void handle(const debug_assert::source_location&, const char*, const char* err_msg, const char* stmt, const char* file, unsigned int line)
    {
      debug_assert::source_location loc;
      loc.file_name = file;
      loc.line_number = line;

      std::string str = std::string(stmt) + " " + std::string(err_msg);

      debug_assert::default_handler::handle(loc, str.data());
    }

  };

  ALenum err;
  err = alGetError();
  DEBUG_ASSERT(err == AL_NO_ERROR, al_assert_handler{}, openal_err2str(err), stmt, file, line);
}

void AudioInstance::init()
{
  AL_CHECK(alGenSources(1, &this->source));

  AL_CHECK(alGenBuffers(MINIGAME_AUDIO_QUEUE_SIZE, this->buffers.data()));
}

void AudioInstance::deinit()
{
  AL_CHECK(alSourceStop(this->source));

  ALint num_queue;
  AL_CHECK(alGetSourcei(this->source, AL_BUFFERS_QUEUED, &num_queue));

  ALuint unqueue_buffers[MINIGAME_AUDIO_QUEUE_SIZE];
  AL_CHECK(alSourceUnqueueBuffers(this->source, num_queue, unqueue_buffers));

  AL_CHECK(alDeleteBuffers(MINIGAME_AUDIO_QUEUE_SIZE, this->buffers.data()));

  AL_CHECK(alDeleteSources(1, &this->source));
}

void AudioInstance::fill_queue_before_play()
{
  auto sp_asset = this->asset.lock();
  DEBUG_ASSERT(sp_asset, assert_handler{});

  AssetAudio audio = std::get<AssetAudio>(*sp_asset);

  ALenum format = (audio.format == AssetAudio::Format::MONO_SHORT16) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

  ALsizei length = sizeof(int16_t) * audio.length * ((format == AL_FORMAT_MONO16) ? 1 : 2);

  for (int i = 0; i < MINIGAME_AUDIO_QUEUE_SIZE; ++i) {
    if (this->readed_bytes == length) { break; }

    ALsizei size = sizeof(uint8_t)*MINIGAME_AUDIO_BUFFER_SIZE;

    if (this->readed_bytes + size >= length) {
      size = length - this->readed_bytes;
    }

    uint8_t* data = (uint8_t*)audio.data;
    AL_CHECK(alBufferData(this->buffers[i], format, data+this->readed_bytes, size, 44100));

    AL_CHECK(alSourceQueueBuffers(this->source, 1, &this->buffers[i]));

    this->readed_bytes += size;
  }
}

void AudioInstance::play()
{
  AL_CHECK(alSourcePlay(this->source));
}

void AudioInstance::update()
{
  auto sp_asset = this->asset.lock();
  DEBUG_ASSERT(sp_asset, assert_handler{});

  AssetAudio audio = std::get<AssetAudio>(*sp_asset);

  ALenum format = (audio.format == AssetAudio::Format::MONO_SHORT16) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

  ALsizei length = sizeof(int16_t) * audio.length * ((format == AL_FORMAT_MONO16) ? 1 : 2);

  ALint num_queue;
  AL_CHECK(alGetSourcei(this->source, AL_BUFFERS_PROCESSED, &num_queue));

  for (int i = 0; i < num_queue; ++i) {
    if (this->readed_bytes == length) { break; }

    ALuint buf;
    AL_CHECK(alSourceUnqueueBuffers(this->source, 1, &buf));

    ALsizei size = sizeof(uint8_t)*MINIGAME_AUDIO_BUFFER_SIZE;

    if (this->readed_bytes + size >= length) {
      size = length - this->readed_bytes;
    }

    uint8_t* data = (uint8_t*)audio.data;
    AL_CHECK(alBufferData(buf, format, data+this->readed_bytes, size, 44100));

    AL_CHECK(alSourceQueueBuffers(this->source, 1, &buf));

    this->readed_bytes += size;
  }

  ALint state;
  AL_CHECK(alGetSourcei(this->source, AL_SOURCE_STATE, &state));

  if (state == AL_STOPPED) { this->is_end_of_stream = true; }
}

void AudioPlayer::thread_for_init()
{
  for (;;) {
    std::optional<std::shared_ptr<AudioInstance>> opt = this->queue.pop();

    if (!opt) { break; }

    std::shared_ptr<AudioInstance> ai = opt.value();

    ai->init();
    ai->fill_queue_before_play();
    ai->play();

    std::lock_guard<std::mutex> lk(this->mtx);

    this->instances.push_back(ai);
  }
}

void AudioPlayer::thread_for_stream()
{
  while (!this->is_close) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    std::lock_guard<std::mutex> lk(this->mtx);

    for (std::shared_ptr<AudioInstance> ai : this->instances) {
      ai->update();
    }

    this->instances.remove_if([](std::shared_ptr<AudioInstance> n) { return n->is_end_of_stream; });
  }
}

std::shared_ptr<AudioInstance> AudioPlayer::play(std::weak_ptr<asset_t> asset)
{
  std::shared_ptr<AudioInstance> ai;

  ai = std::make_shared<AudioInstance>();
  ai->asset = asset;

  this->queue.push(ai);

  return ai;
}

#endif // MINIGAME_SRC_AUDIO_AUDIO_PLAYER_HPP_INCLUDED
