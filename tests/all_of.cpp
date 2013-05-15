#include <accent/algorithms/all_of.hpp>
#include "test.hpp"

TEST(AllOf, emptyRange_isTrue)
{
  vec v = {};
  auto r = v.sp();
  ASSERT_TRUE(all_of(r, positive()));
}

TEST(AllOf, singleFalse_isFalse)
{
  vec v = { -1 };
  auto r = v.sp();
  ASSERT_FALSE(all_of(r, positive()));
}

TEST(AllOf, singleTrue_isTrue)
{
  vec v = { 1 };
  auto r = v.sp();
  ASSERT_TRUE(all_of(r, positive()));
}

TEST(AllOf, trueFalse_isFalse)
{
  vec v = { 1, -1 };
  auto r = v.sp();
  ASSERT_FALSE(all_of(r, positive()));
}

TEST(AllOf, falseTrue_isFalse)
{
  vec v = { -1, 1 };
  auto r = v.sp();
  ASSERT_FALSE(all_of(r, positive()));
}

TEST(AllOf, trueTrue_isTrue)
{
  vec v = { 1, 1 };
  auto r = v.sp();
  ASSERT_TRUE(all_of(r, positive()));
}

TEST(AllOf, falseFalse_isFalse)
{
  vec v = { -1, -1 };
  auto r = v.sp();
  ASSERT_FALSE(all_of(r, positive()));
}

TEST(AllOf, falseTrue_abortsEarly) {
  int calls = 0;
  vec v = { -1, 1 };
  auto r = v.sp();
  all_of(r, tracking_positive(calls));
  ASSERT_EQ(1, calls);
}
