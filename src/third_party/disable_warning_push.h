#ifndef MINIGAME_SRC_THIRD_PARTY_DISABLE_WARNING_PUSH_H_INCLUDED
#define MINIGAME_SRC_THIRD_PARTY_DISABLE_WARNING_PUSH_H_INCLUDED

#ifdef _MSC_VER

#pragma warning(push)

#pragma warning(disable: 4100)
#pragma warning(disable: 4152)
#pragma warning(disable: 4204)
#pragma warning(disable: 4244)
#pragma warning(disable: 4245)
#pragma warning(disable: 4456)
#pragma warning(disable: 4457)
#pragma warning(disable: 4457)
#pragma warning(disable: 4701)
#pragma warning(disable: 4996)

#endif // _MSC_VER

#ifdef __GNUC__

#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-value"
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wcast-function-type"

#endif // __GNUC__

#endif // MINIGAME_SRC_THIRD_PARTY_DISABLE_WARNING_PUSH_H_INCLUDED
