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
using concat_fwd_bidi = decltype(concat(vec{}.fwd(), vec{}.bidi()));
using concat_bidi_fwd = decltype(concat(vec{}.bidi(), vec{}.fwd()));

static_assert(ForwardRange<concat_fwd_fwd>(),
              "concatenation of 2 foward ranges is not forward range");
static_assert(ForwardRange<concat_fwd_bidi>(),
              "concatenation of foward and bidirectional range "
              "is not forward range");
static_assert(ForwardRange<concat_bidi_fwd>(),
              "concatenation of bidirectional and foward range "
              "is not forward range");
static_assert(!BidirectionalRange<concat_fwd_bidi>(),
              "concatenation of foward and bidirectional range "
              "claims to be bidirectional range");
static_assert(!BidirectionalRange<concat_bidi_fwd>(),
              "concatenation of bidirectional and foward range "
              "claims to be bidirectional range");

using concat_bidi_bidi = decltype(concat(vec{}.bidi(), vec{}.bidi()));

static_assert(BidirectionalRange<concat_bidi_bidi>(),
              "concatenation of 2 bidirectional ranges "
              "is not bidirectional range");

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

TEST(Concat, OnePlusEmpty_OneBack) {
  vec v1 = { 1 }, v2 = {};
  auto r = concat(v1.bidi(), v2.bidi());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(1, r.back());
  r.drop_back();
  ASSERT_TRUE(r.empty());
}

TEST(Concat, EmptyPlusOne_OneBack) {
  vec v1 = {}, v2 = { 1 };
  auto r = concat(v1.bidi(), v2.bidi());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(1, r.back());
  r.drop_back();
  ASSERT_TRUE(r.empty());
}

TEST(Concat, OnePlusOne_TwoBack) {
  vec v1 = { 1 }, v2 = { 2 };
  auto r = concat(v1.bidi(), v2.bidi());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(2, r.back());
  r.drop_back();
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(1, r.back());
  r.drop_back();
  ASSERT_TRUE(r.empty());
}

TEST(Concat, TwoPlusOne_ThreeBack) {
  vec v1 = { 1, 2 }, v2 = { 3 };
  auto r = concat(v1.bidi(), v2.bidi());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(3, r.back());
  r.drop_back();
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(2, r.back());
  r.drop_back();
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(1, r.back());
  r.drop_back();
  ASSERT_TRUE(r.empty());
}

TEST(Concat, EmptyPlusEmpty_AtBackIsInvalid) {
  vec v1 = {}, v2 = {};
  auto r = concat(v1.bidi(), v2.bidi());
  ASSERT_FALSE(r.at_back().valid());
}

TEST(Concat, OnePlusEmpty_AtBackIsValid) {
  vec v1 = { 1 }, v2 = {};
  auto r = concat(v1.bidi(), v2.bidi());
  ASSERT_TRUE(r.at_back().valid());
}

TEST(Concat, OnePlusEmpty_BackAtOne) {
  vec v1 = { 1 }, v2 = {};
  auto r = concat(v1.bidi(), v2.bidi());
  ASSERT_EQ(1, *r.at_back());
}

TEST(Concat, EmptyPlusOne_BackAtOne) {
  vec v1 = {}, v2 = { 1 };
  auto r = concat(v1.bidi(), v2.bidi());
  ASSERT_EQ(1, *r.at_back());
}

TEST(Concat, OnePlusOne_BackAtTwo) {
  vec v1 = { 1 }, v2 = { 2 };
  auto r = concat(v1.bidi(), v2.bidi());
  ASSERT_EQ(2, *r.at_back());
}

TEST(Concat, OnePlusOne_DropBack_BackAtOne) {
  vec v1 = { 1 }, v2 = { 2 };
  auto r = concat(v1.bidi(), v2.bidi());
  r.drop_back();
  ASSERT_EQ(1, *r.at_back());
}

TEST(Concat, OnePlusTwo_SetBackToSecond_TwoRemaining) {
  vec v1 = { 1 }, v2 = { 2, 3 };
  auto r = concat(v1.bidi(), v2.bidi());
  auto s = r;
  s.drop_front();
  r.set_back(s.at_front());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(2, r.back());
  r.drop_back();
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(1, r.back());
  r.drop_back();
  ASSERT_TRUE(r.empty());
}

TEST(Concat, OnePlusOne_SetBackToInvalid_Empty) {
  vec v1 = { 1 }, v2 = { 2 };
  auto r = concat(v1.bidi(), v2.bidi());
  r.set_back(decltype(r.at_front()){});
  ASSERT_TRUE(r.empty());
}
