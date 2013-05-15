#include <accent/algorithms/any_of.hpp>
#include "test.hpp"

TEST(AnyOf, emptyRange_isFalse)
{
  vec v = {};
  auto r = v.sp();
  ASSERT_FALSE(any_of(r, positive()));
}
