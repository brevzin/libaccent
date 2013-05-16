#include <accent/algorithms/none_of.hpp>
#include "test.hpp"

TEST(NoneOf, emptyRange_isTrue)
{
  vec v = {};
  auto r = v.sp();
  ASSERT_TRUE(none_of(r, positive()));
}

TEST(NoneOf, singleFalse_isTrue)
{
  vec v = { -1 };
  auto r = v.sp();
  ASSERT_TRUE(none_of(r, positive()));
}

TEST(NoneOf, singleTrue_isFalse)
{
  vec v = { 1 };
  auto r = v.sp();
  ASSERT_FALSE(none_of(r, positive()));
}

TEST(NoneOf, trueFalse_isFalse)
{
  vec v = { 1, -1 };
  auto r = v.sp();
  ASSERT_FALSE(none_of(r, positive()));
}

TEST(NoneOf, falseTrue_isFalse)
{
  vec v = { -1, 1 };
  auto r = v.sp();
  ASSERT_FALSE(none_of(r, positive()));
}

TEST(NoneOf, trueTrue_isFalse)
{
  vec v = { 1, 1 };
  auto r = v.sp();
  ASSERT_FALSE(none_of(r, positive()));
}

TEST(NoneOf, falseFalse_isTrue)
{
  vec v = { -1, -1 };
  auto r = v.sp();
  ASSERT_TRUE(none_of(r, positive()));
}

TEST(NoneOf, trueFalse_abortsEarly) {
  int calls = 0;
  vec v = { 1, -1 };
  auto r = v.sp();
  none_of(r, tracking_positive(calls));
  ASSERT_EQ(1, calls);
}
