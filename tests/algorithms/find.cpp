#include <accent/algorithms/find.hpp>
#include "test.hpp"

TEST(Find, emptyRange_noPosition) {
  vec v;
  auto r = v.fwd();
  ASSERT_FALSE(find(r, positive()).valid());
}

TEST(Find, oneWrongElement_noPosition) {
  vec v = { -1 };
  auto r = v.fwd();
  ASSERT_FALSE(find(r, positive()).valid());
}

TEST(Find, twoWrongElements_noPosition) {
  vec v = { -1, -2 };
  auto r = v.fwd();
  ASSERT_FALSE(find(r, positive()).valid());
}

TEST(Find, oneRightElement_positionOfElement) {
  vec v = { 1 };
  auto r = v.fwd();
  auto pos = find(r, positive());
  ASSERT_TRUE(pos.valid());
  ASSERT_EQ(1, *pos);
}

TEST(Find, centeredRightElement_positionOfElement) {
  vec v = { -1, 2, -3 };
  auto r = v.fwd();
  auto pos = find(r, positive());
  ASSERT_TRUE(pos.valid());
  ASSERT_EQ(2, *pos);
}
