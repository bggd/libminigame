#ifndef MINIGAME_SRC_THIRD_PARTY_DEBUG_ASSERT_HPP_INCLUDED
#define MINIGAME_SRC_THIRD_PARTY_DEBUG_ASSERT_HPP_INCLUDED

#include "../../third_party/debug_assert/debug_assert.hpp"

struct  assert_handler : debug_assert::default_handler, debug_assert::set_level<-1> {
};

#endif // MINIGAME_SRC_THIRD_PARTY_DEBUG_ASSERT_HPP_INCLUDED
