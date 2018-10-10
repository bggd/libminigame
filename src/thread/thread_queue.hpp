#ifndef MINIGAME_SRC_THREAD_QUEUE_HPP_INCLUDED
#define MINIGAME_SRC_THREAD_QUEUE_HPP_INCLUDED

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <stdio.h>


template <typename T>
struct ThreadQueue {

  std::mutex mtx;
  std::condition_variable cv;
  std::queue<T> queue;

  void push(T v) noexcept;
  T pop() noexcept;

};

template <typename T>
void ThreadQueue<T>::push(T v) noexcept
{
  try {
    std::lock_guard<std::mutex> lk(this->mtx);

    this->queue.push(v);
    this->cv.notify_one();
  }
  catch (const std::exception& e) {
    fprintf(stderr, "%s\n", e.what());
    abort();
  }
}

template <typename T>
T ThreadQueue<T>::pop() noexcept
{
  try {
    std::unique_lock<std::mutex> lk(this->mtx);

    while (this->queue.empty()) {
      this->cv.wait(lk);
    }

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
