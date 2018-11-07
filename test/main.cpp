#include <boost/core/lightweight_test.hpp>

#include "thread/test_thread_queue.hpp"
#include "asset/test_asset_loader.hpp"
#include "audio/test_audio_player.hpp"


int main()
{
  test_thread_queue();
  test_asset_loader();
  test_audio_player();
  return boost::report_errors();
}
