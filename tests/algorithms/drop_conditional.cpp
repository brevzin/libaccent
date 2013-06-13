#include <accent/algorithms/drop_conditional.hpp>
#include "test.hpp"

TEST(DropWhile, EmptyRange_StillEmpty) {
  vec v;
  auto r = drop_while(v.sp(), positive());
  ASSERT_TRUE(r.empty());
}

TEST(DropWhile, OneUndroppedElement_ElementRemains) {
  vec v = { -1 };
  auto r = drop_while(v.sp(), positive());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(-1, r.front());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}

TEST(DropWhile, OneDroppedElement_BecomesEmpty) {
  vec v = { 1 };
  auto r = drop_while(v.sp(), positive());
  ASSERT_TRUE(r.empty());
}

TEST(DropWhile, Undropped_Dropped_StaysSame) {
  vec v = { -1, 1 };
  auto r = drop_while(v.sp(), positive());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(-1, r.front());
  r.drop_front();
  ASSERT_EQ(1, r.front());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}

TEST(DropWhile, Dropped_Undropped_OneRemains) {
  vec v = { 1, -1 };
  auto r = drop_while(v.sp(), positive());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(-1, r.front());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}

TEST(DropWhile, Dropped_Dropped_Undropped_OneRemains) {
  vec v = { 1, 1, -1 };
  auto r = drop_while(v.sp(), positive());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(-1, r.front());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}

TEST(DropWhile, Dropped_Undropped_Dropped_TwoRemain) {
  vec v = { 1, -1, 1 };
  auto r = drop_while(v.sp(), positive());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(-1, r.front());
  r.drop_front();
  ASSERT_EQ(1, r.front());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}
