#include <boost/core/lightweight_test.hpp>

#include "thread/test_thread_queue.hpp"
#include "asset/test_asset_loader.hpp"

#define STBI_ONLY_TGA
#define STB_IMAGE_IMPLEMENTATION

#ifdef _MSC_VER
  #pragma warning(push)
  #pragma warning(disable : 4100)
  #pragma warning(disable : 4505)
  #include "../third_party/stb/stb_image.h"
  #pragma warning(pop)
#elif __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunused-parameter"
  #pragma GCC diagnostic ignored "-Wunused-function"
  #include "../third_party/stb/stb_image.h"
  #pragma GCC diagnostic pop
#else
  #include "../third_party/stb/stb_image.h"
#endif

int main()
{
  test_thread_queue();
  test_asset_loader();
  return boost::report_errors();
}
