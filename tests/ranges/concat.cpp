#include <accent/ranges/concat.hpp>
#include "test.hpp"

using concat_sp_sp = decltype(concat(vec{}.sp(), vec{}.sp()));
using concat_sp_sp_sp = decltype(concat(vec{}.sp(), vec{}.sp(), vec{}.sp()));
using concat_sp_fwd = decltype(concat(vec{}.sp(), vec{}.fwd()));
using concat_fwd_sp = decltype(concat(vec{}.fwd(), vec{}.sp()));

static_assert(SinglePassRange<concat_sp_sp>(),
              "concatenation of 2 single pass ranges is not single pass range");
static_assert(SinglePassRange<concat_sp_sp_sp>(),
              "concatenation of 3 single pass ranges is not single pass range");
static_assert(SinglePassRange<concat_sp_fwd>(),
              "concatenation of single pass and forward range "
              "is not single pass range");
static_assert(!ForwardRange<concat_sp_fwd>(),
              "concatenation of single pass and forward range "
              "claims to be forward range");
static_assert(!ForwardRange<concat_fwd_sp>(),
              "concatenation of forward and single pass range "
              "claims to be forward range");

using concat_fwd_fwd = decltype(concat(vec{}.fwd(), vec{}.fwd()));

static_assert(ForwardRange<concat_fwd_fwd>(),
              "concatenation of 2 foward ranges is not forward range");

TEST(Concat, EmptyPlusEmpty_Empty) {
  vec v1 = {}, v2 = {};
  auto r = concat(v1.sp(), v2.sp());
  ASSERT_TRUE(r.empty());
}

TEST(Concat, OnePlusEmpty_One) {
  vec v1 = { 1 }, v2 = {};
  auto r = concat(v1.sp(), v2.sp());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(1, r.front());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}

TEST(Concat, EmptyPlusOne_One) {
  vec v1 = {}, v2 = { 1 };
  auto r = concat(v1.sp(), v2.sp());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(1, r.front());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}

TEST(Concat, OnePlusOne_Two) {
  vec v1 = { 1 }, v2 = { 2 };
  auto r = concat(v1.sp(), v2.sp());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(1, r.front());
  r.drop_front();
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(2, r.front());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}

TEST(Concat, TwoPlusOne_Three) {
  vec v1 = { 1, 2 }, v2 = { 3 };
  auto r = concat(v1.sp(), v2.sp());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(1, r.front());
  r.drop_front();
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(2, r.front());
  r.drop_front();
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(3, r.front());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}

TEST(Concat, OnePlusOnePlusOne_Three) {
  vec v1 = { 1 }, v2 = { 2 }, v3 = { 3 };
  auto r = concat(v1.sp(), v2.sp(), v3.sp());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(1, r.front());
  r.drop_front();
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(2, r.front());
  r.drop_front();
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(3, r.front());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}

TEST(Concat, EmptyPlusEmpty_AtFrontIsInvalid) {
  vec v1 = {}, v2 = {};
  auto r = concat(v1.fwd(), v2.fwd());
  ASSERT_FALSE(r.at_front().valid());
}

TEST(Concat, EmptyPlusEmpty_AtFrontEqualsSelf) {
  vec v1 = {}, v2 = {};
  auto r = concat(v1.fwd(), v2.fwd());
  auto p = r.at_front();
  ASSERT_TRUE(p == p);
}

TEST(Concat, OnePlusEmpty_AtFrontIsValid) {
  vec v1 = { 1 }, v2 = {};
  auto r = concat(v1.fwd(), v2.fwd());
  ASSERT_TRUE(r.at_front().valid());
}

TEST(Concat, OnePlusEmpty_AtFrontEqualsSelf) {
  vec v1 = { 1 }, v2 = {};
  auto r = concat(v1.fwd(), v2.fwd());
  auto p = r.at_front();
  ASSERT_TRUE(p == p);
}

TEST(Concat, OnePlusEmpty_AtFrontNotEqualsEmpty) {
  vec v1 = { 1 }, v2 = {};
  auto r = concat(v1.fwd(), v2.fwd());
  auto p = r.at_front();
  ASSERT_TRUE(p != decltype(p){});
}

TEST(Concat, OnePlusEmpty_AtOne) {
  vec v1 = { 1 }, v2 = {};
  auto r = concat(v1.fwd(), v2.fwd());
  ASSERT_EQ(1, *r.at_front());
}

TEST(Concat, EmptyPlusOne_AtOne) {
  vec v1 = {}, v2 = { 1 };
  auto r = concat(v1.fwd(), v2.fwd());
  ASSERT_EQ(1, *r.at_front());
}

TEST(Concat, OnePlusOne_AtOne) {
  vec v1 = { 1 }, v2 = { 2 };
  auto r = concat(v1.fwd(), v2.fwd());
  ASSERT_EQ(1, *r.at_front());
}

TEST(Concat, OnePlusOne_DropFront_AtTwo) {
  vec v1 = { 1 }, v2 = { 2 };
  auto r = concat(v1.fwd(), v2.fwd());
  r.drop_front();
  ASSERT_EQ(2, *r.at_front());
}

TEST(Concat, TwoPlusOne_SetFrontToSecond_TwoRemaining) {
  vec v1 = { 1, 2 }, v2 = { 3 };
  auto r = concat(v1.fwd(), v2.fwd());
  auto s = r;
  s.drop_front();
  r.set_front(s.at_front());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(2, r.front());
  r.drop_front();
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(3, r.front());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}

TEST(Concat, OnePlusOne_SetFrontToInvalid_Empty) {
  vec v1 = { 1 }, v2 = { 2 };
  auto r = concat(v1.fwd(), v2.fwd());
  r.set_front(decltype(r.at_front()){});
  ASSERT_TRUE(r.empty());
}
