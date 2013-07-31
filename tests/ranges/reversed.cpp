#include <accent/ranges/reversed.hpp>
#include "test.hpp"

static_assert(BidirectionalRange<decltype(reversed(vec{}.bidi()))>(),
              "reversed bidirectional range is not bidirectional range");

TEST(Reversed, SourceEmpty_TargetEmpty) {
  vec v = {};
  auto r = reversed(v.bidi());
  ASSERT_TRUE(r.empty());
}

TEST(Reversed, OneElement_FrontIsElement) {
  vec v = { 1 };
  auto r = reversed(v.bidi());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(1, r.front());
}

TEST(Reversed, OneElement_BackIsElement) {
  vec v = { 1 };
  auto r = reversed(v.bidi());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(1, r.back());
}

TEST(Reversed, TwoElements_FrontIsSecond) {
  vec v = { 1, 2 };
  auto r = reversed(v.bidi());
  ASSERT_EQ(2, r.front());
}

TEST(Reversed, TwoElements_BackIsFirst) {
  vec v = { 1, 2 };
  auto r = reversed(v.bidi());
  ASSERT_EQ(1, r.back());
}

TEST(Reversed, OneElement_DropFront_Empty) {
  vec v = { 1 };
  auto r = reversed(v.bidi());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}

TEST(Reversed, OneElement_DropBack_Empty) {
  vec v = { 1 };
  auto r = reversed(v.bidi());
  r.drop_back();
  ASSERT_TRUE(r.empty());
}

TEST(Reversed, TwoElements_DropFront_FrontIsFirst) {
  vec v = { 1, 2 };
  auto r = reversed(v.bidi());
  r.drop_front();
  ASSERT_EQ(1, r.front());
}

TEST(Reversed, TwoElements_DropBack_BackIsSecond) {
  vec v = { 1, 2 };
  auto r = reversed(v.bidi());
  r.drop_back();
  ASSERT_EQ(2, r.back());
}

TEST(Reversed, Empty_AtFrontIsInvalid) {
  vec v = {};
  auto r = reversed(v.bidi());
  ASSERT_FALSE(r.at_front().valid());
}

TEST(Reversed, Empty_AtBackIsInvalid) {
  vec v = {};
  auto r = reversed(v.bidi());
  ASSERT_FALSE(r.at_back().valid());
}

TEST(Reversed, TwoElements_AtFrontIsSecond) {
  vec v = { 1, 2 };
  auto r = reversed(v.bidi());
  auto p = r.at_front();
  ASSERT_TRUE(p.valid());
  ASSERT_EQ(2, *p);
}

TEST(Reversed, TwoElements_AtBackIsFirst) {
  vec v = { 1, 2 };
  auto r = reversed(v.bidi());
  auto p = r.at_back();
  ASSERT_TRUE(p.valid());
  ASSERT_EQ(1, *p);
}

TEST(Reversed, ThreeElements_SetFrontToCenter_HasFirstTwo) {
  vec v = { 1, 2, 3 };
  auto r = reversed(v.bidi());
  auto s = r;
  s.drop_back();
  r.set_front(s.at_back());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(2, r.front());
  r.drop_front();
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(1, r.front());
}

TEST(Reversed, ThreeElements_SetBackToCenter_HasLastTwo) {
  vec v = { 1, 2, 3 };
  auto r = reversed(v.bidi());
  auto s = r;
  s.drop_front();
  r.set_back(s.at_front());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(2, r.back());
  r.drop_back();
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(3, r.back());
}
