#include <accent/ranges/take_conditional.hpp>
#include "test.hpp"

static_assert(SinglePassRange<decltype(take_until(vec{}.sp(), positive()))>(),
              "take wrapper of single pass range is not single pass range");
static_assert(!ForwardRange<decltype(take_until(vec{}.sp(), positive()))>(),
              "take wrapper of single pass range claims forward range");
static_assert(ForwardRange<decltype(take_until(vec{}.fwd(), positive()))>(),
              "take wrapper of forward range is not forward range");
static_assert(!BidirectionalRange<
                  decltype(take_until(vec{}.fwd(), positive()))>(),
              "take wrapper of forward range claims bidirectional range");
static_assert(!BidirectionalRange<
                  decltype(take_until(vec{}.fwd(), positive()))>(),
              "take wrapper of bidirectional range claims bidirectional range");

TEST(TakeUntil, EmptyRange_StillEmpty) {
  vec v;
  auto r = take_until(v.sp(), positive());
  ASSERT_TRUE(r.empty());
}

TEST(TakeUntil, OneTakenElement_ElementRemains) {
  vec v = { -1 };
  auto r = take_until(v.sp(), positive());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(-1, r.front());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}

TEST(TakeUntil, OneUntakenElement_BecomesEmpty) {
  vec v = { 1 };
  auto r = take_until(v.sp(), positive());
  ASSERT_TRUE(r.empty());
}

TEST(TakeUntil, Untaken_Taken_BecomesEmpty) {
  vec v = { 1, -1 };
  auto r = take_until(v.sp(), positive());
  ASSERT_TRUE(r.empty());
}

TEST(TakeUntil, Taken_Untaken_OneRemains) {
  vec v = { -1, 1 };
  auto r = take_until(v.sp(), positive());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(-1, r.front());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}

TEST(TakeUntil, EmptyRange_AtFrontInvalid) {
  vec v = {};
  auto r = take_until(v.fwd(), positive());
  ASSERT_FALSE(r.at_front().valid());
}

TEST(TakeUntil, OneUntakenElement_AtFrontInvalid) {
  vec v = { 1 };
  auto r = take_until(v.fwd(), positive());
  ASSERT_FALSE(r.at_front().valid());
}

TEST(TakeUntil, OneTakenElement_AtFrontIsElement) {
  vec v = { -1 };
  auto r = take_until(v.fwd(), positive());
  auto p = r.at_front();
  ASSERT_TRUE(p.valid());
  ASSERT_EQ(-1, *p);
}

TEST(TakeUntil, TwoTakenElements_SetFrontToSecond_OneRemains) {
  vec v = { -1, -2 };
  auto r = take_until(v.fwd(), positive());
  auto s = r;
  s.drop_front();
  r.set_front(s.at_front());
  ASSERT_FALSE(r.empty());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}
