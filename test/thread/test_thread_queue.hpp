#ifndef MINIGAME_TEST_TEST_THREAD_THREAD_QUEUE_HPP_INCLUDED
#define MINIGAME_TEST_TEST_THREAD_THREAD_QUEUE_HPP_INCLUDED

#include <gtest/gtest.h>

#include "../../src/thread/thread_queue.hpp"

#include <stdint.h>
#include <stdlib.h>
#include <thread>
#include <chrono>


void other_thread_1(ThreadQueue<uint32_t>* queue)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  for (uint32_t i = 0; i < 100; ++i) {
    queue->push(i);
  }
}

void other_thread_2(ThreadQueue<uint32_t>* queue)
{
  uint32_t num = 0;
  for (uint32_t i = 0; i < 10*100; ++i) {
    num += queue->pop().value();

  }
  queue->push(num);
}

TEST(ThreadQueue, Test)
{
  ThreadQueue<uint32_t> queue;

  queue.push(0);

  ASSERT_TRUE(queue.pop().value() == 0);

  std::thread th(other_thread_2, &queue);
  std::thread threads[10];
  for (uint32_t i = 0; i < 10; ++i) {
    threads[i] = std::thread(other_thread_1, &queue);
  }
  for (uint32_t i = 0; i < 10; ++i) {
    threads[i].join();
  }
  th.join();

  ASSERT_FALSE(queue.is_empty());
  ASSERT_TRUE(queue.pop().value() == 49500);
  ASSERT_TRUE(queue.is_empty());

  ASSERT_FALSE(queue.is_close);
  queue.close();
  ASSERT_TRUE(queue.is_close);
  std::optional<uint32_t> opt = queue.pop();
  ASSERT_FALSE(opt.has_value());

  ThreadQueue<const char*> queue_cstr;
  
  queue_cstr.push("foo.tga");
  queue_cstr.push("bar.wav");

  ASSERT_STREQ(queue_cstr.pop().value(), "foo.tga");
  ASSERT_STREQ(queue_cstr.pop().value(), "bar.wav");
}

#endif // MINIGAME_TEST_TEST_THREAD_THREAD_QUEUE_HPP_INCLUDED
