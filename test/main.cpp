#include <boost/core/lightweight_test.hpp>

#include "thread/test_thread_queue.hpp"
#include "asset/test_asset_loader.hpp"


int main()
{
  test_thread_queue();
  test_asset_loader();
  return boost::report_errors();
}
