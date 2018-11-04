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

  bool is_empty() noexcept;
  void close() noexcept;
  void push(T v) noexcept;
  std::optional<T> pop() noexcept;

};

template <typename T>
bool ThreadQueue<T>::is_empty() noexcept
{
  try {
    std::lock_guard<std::mutex> lk(this->mtx);
    return this->queue.empty();
  }
  catch (const std::exception& e) {
    fprintf(stderr, "%s\n", e.what());
    abort();
  }
}

template <typename T>
void ThreadQueue<T>::close() noexcept
{
  this->is_close = true;
  this->cv.notify_all();
}

template <typename T>
void ThreadQueue<T>::push(T v) noexcept
{
  try {
    std::lock_guard<std::mutex> lk(this->mtx);

    this->queue.push(v);
  }
  catch (const std::exception& e) {
    fprintf(stderr, "%s\n", e.what());
    abort();
  }
  this->cv.notify_one();
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

    T v = this->queue.front();
    this->queue.pop();

    return v;
  }
  catch (const std::exception& e) {
    fprintf(stderr, "%s\n", e.what());
    abort();
  }
}


#endif // MINIGAME_SRC_THREAD_QUEUE_HPP_INCLUDED
