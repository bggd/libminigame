#ifndef MINIGAME_SRC_THREAD_QUEUE_HPP_INCLUDED
#define MINIGAME_SRC_THREAD_QUEUE_HPP_INCLUDED

#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <optional>
#include <stdio.h>


template <typename T>
struct ThreadQueue {

  std::mutex mtx;
  std::condition_variable cv;
  std::queue<T> queue;
  std::atomic<bool> is_close = false;

  void close();
  void push(T v) noexcept;
  std::optional<T> pop() noexcept;

};

template <typename T>
void ThreadQueue<T>::close()
{
  this->is_close = true;
  this->cv.notify_all();
}

template <typename T>
void ThreadQueue<T>::push(T v) noexcept
{
  try {
    std::lock_guard<std::mutex> lk(this->mtx);

    this->queue.push(std::move(v));
    this->cv.notify_one();
  }
  catch (const std::exception& e) {
    fprintf(stderr, "%s\n", e.what());
    abort();
  }
}

template <typename T>
std::optional<T> ThreadQueue<T>::pop() noexcept
{
  if (this->is_close) { return std::nullopt; }

  try {
    std::unique_lock<std::mutex> lk(this->mtx);

    while (!this->is_close && this->queue.empty()) {
      this->cv.wait(lk);
    }

    if (this->is_close) { return std::nullopt; }

    T&& v(std::move(this->queue.front()));
    this->queue.pop();

    return std::move(v);
  }
  catch (const std::exception& e) {
    fprintf(stderr, "%s\n", e.what());
    abort();
  }
}


#endif // MINIGAME_SRC_THREAD_QUEUE_HPP_INCLUDED
