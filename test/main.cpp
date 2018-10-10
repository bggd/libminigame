#include <boost/core/lightweight_test.hpp>

#include "thread/test_thread_queue.hpp"

int main()
{
  test_thread_queue();
  return boost::report_errors();
}
