#ifndef MINIGAME_TEST_TEST_THREAD_THREAD_QUEUE_HPP_INCLUDED
#define MINIGAME_TEST_TEST_THREAD_THREAD_QUEUE_HPP_INCLUDED

#include <boost/core/lightweight_test.hpp>

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

void test_thread_queue()
{
  ThreadQueue<uint32_t> queue;

  queue.push(0);

  BOOST_TEST_EQ(queue.pop().value(), 0);

  std::thread th(other_thread_2, &queue);
  std::thread threads[10];
  for (uint32_t i = 0; i < 10; ++i) {
    threads[i] = std::thread(other_thread_1, &queue);
  }
  for (uint32_t i = 0; i < 10; ++i) {
    threads[i].join();
  }
  th.join();

  BOOST_TEST_EQ(queue.pop().value(), 49500);
  BOOST_TEST(queue.queue.empty());

  ThreadQueue<const char*> queue_cstr;
  
  queue_cstr.push("foo.tga");
  queue_cstr.push("bar.wav");

  BOOST_TEST_CSTR_EQ(queue_cstr.pop().value(), "foo.tga");
  BOOST_TEST_CSTR_EQ(queue_cstr.pop().value(), "bar.wav");
}

#endif // MINIGAME_TEST_TEST_THREAD_THREAD_QUEUE_HPP_INCLUDED
