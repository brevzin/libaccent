#include <accent/algorithms/for_each.hpp>
#include "test.hpp"

struct accumulate {
  int sum = 0;
  void operator ()(int i) { sum += i; }
};

TEST(ForEach, Accumulate) {
  vec v = { 1, 2, 3 };
  int sum = for_each(v.sp(), accumulate()).sum;
  ASSERT_EQ(6, sum);
}

TEST(ForEach, Modify) {
  vec v = { 1, 2, 3 };
  for_each(v.sp(), [](int& i) { ++i; });
  ASSERT_EQ((vec{ 2, 3, 4 }), v);
}
