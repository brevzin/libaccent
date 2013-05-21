#include <accent/algorithms/any_of.hpp>
#include "test.hpp"

TEST(AnyOf, emptyRange_isFalse)
{
  vec v = {};
  auto r = v.sp();
  ASSERT_FALSE(any_of(r, positive()));
}

TEST(AnyOf, singleFalse_isFalse)
{
  vec v = { -1 };
  auto r = v.sp();
  ASSERT_FALSE(any_of(r, positive()));
}

TEST(AnyOf, singleTrue_isTrue)
{
  vec v = { 1 };
  auto r = v.sp();
  ASSERT_TRUE(any_of(r, positive()));
}

TEST(AnyOf, trueFalse_isTrue)
{
  vec v = { 1, -1 };
  auto r = v.sp();
  ASSERT_TRUE(any_of(r, positive()));
}

TEST(AnyOf, falseTrue_isTrue)
{
  vec v = { -1, 1 };
  auto r = v.sp();
  ASSERT_TRUE(any_of(r, positive()));
}

TEST(AnyOf, trueTrue_isTrue)
{
  vec v = { 1, 1 };
  auto r = v.sp();
  ASSERT_TRUE(any_of(r, positive()));
}

TEST(AnyOf, falseFalse_isFalse)
{
  vec v = { -1, -1 };
  auto r = v.sp();
  ASSERT_FALSE(any_of(r, positive()));
}

TEST(AnyOf, trueFalse_abortsEarly) {
  int calls = 0;
  vec v = { 1, -1 };
  auto r = v.sp();
  any_of(r, tracking_positive(calls));
  ASSERT_EQ(1, calls);
}
