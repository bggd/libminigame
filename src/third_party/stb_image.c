#include "disable_warning_push.h"

#define STBI_NO_JPEG
#define STBI_NO_PNG
#define STBI_NO_BMP
#define STBI_NO_PSD
#define STBI_NO_GIF
#define STBI_NO_HDR
#define STBI_NO_PIC
#define STBI_NO_PNM

#define STBI_NO_STDIO
#define STB_IMAGE_IMPLEMENTATION
#include "../../third_party/stb/stb_image.h"

#include "disable_warning_pop.h"
