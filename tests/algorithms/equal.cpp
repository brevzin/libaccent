#include <accent/algorithms/equal.hpp>
#include "test.hpp"

TEST(Equal, EmptyRange_EmptyRange_True) {
  vec v1 = {}, v2 = {};
  auto r1 = v1.sp();
  auto r2 = v2.fwd();
  ASSERT_TRUE(equal(r1, r2));
}

TEST(Equal, EmptyRange_OneElement_False) {
  vec v1 = {}, v2 = { 1 };
  auto r1 = v1.sp();
  auto r2 = v2.fwd();
  ASSERT_FALSE(equal(r1, r2));
}

TEST(Equal, OneElement_SameElement_True) {
  vec v1 = { 1 }, v2 = { 1 };
  auto r1 = v1.sp();
  auto r2 = v2.fwd();
  ASSERT_TRUE(equal(r1, r2));
}

TEST(Equal, OneElement_DifferentElement_False) {
  vec v1 = { 0 }, v2 = { 1 };
  auto r1 = v1.sp();
  auto r2 = v2.fwd();
  ASSERT_FALSE(equal(r1, r2));
}

TEST(Equal, TwoElements_SecondDiffers_False) {
  vec v1 = { 0, -1 }, v2 = { 0, 1 };
  auto r1 = v1.sp();
  auto r2 = v2.fwd();
  ASSERT_FALSE(equal(r1, r2));
}

TEST(Equal, TwoElements_SameElements_True) {
  vec v1 = { 1, 2 }, v2 = { 1, 2 };
  auto r1 = v1.sp();
  auto r2 = v2.fwd();
  ASSERT_TRUE(equal(r1, r2));
}
