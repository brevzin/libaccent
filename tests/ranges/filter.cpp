#include <accent/ranges/filter.hpp>
#include "test.hpp"

template <typename Range>
auto get(Range r) -> decltype(filter(r, positive())) {
  return filter(r, positive());
}

static_assert(SinglePassRange<decltype(get(vec{}.sp()))>(),
              "filter wrapper of single pass range is not single pass range");
static_assert(!ForwardRange<decltype(get(vec{}.sp()))>(),
              "filter wrapper of single pass range claims forward range");
static_assert(ForwardRange<decltype(get(vec{}.fwd()))>(),
              "filter wrapper of forward range is not forward range");
static_assert(!BidirectionalRange<decltype(get(vec{}.fwd()))>(),
              "filter wrapper of forward range claims bidirectional range");
static_assert(BidirectionalRange<decltype(get(vec{}.bidi()))>(),
            "filter wrapper of bidirectional range is not bidirectional range");

TEST(Filter, EmptyInput_EmptyOutput) {
  vec v = {};
  auto r = get(v.sp());
  ASSERT_TRUE(r.empty());
}

TEST(Filter, OneRejectedElement_EmptyOutput) {
  vec v = { -1 };
  auto r = get(v.sp());
  ASSERT_TRUE(r.empty());
}

TEST(Filter, OneAcceptedElement_OneElementOutput) {
  vec v = { 1 };
  auto r = get(v.sp());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(1, r.front());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}

TEST(Filter, AcceptedRejected_OneElementOutput) {
  vec v = { 1, -2 };
  auto r = get(v.sp());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(1, r.front());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}

TEST(Filter, RejectedAccepted_OneElementOutput) {
  vec v = { -1, 2 };
  auto r = get(v.sp());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(2, r.front());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}

TEST(Filter, AcceptedRejectedAccepted_TwoElementsOutput) {
  vec v = { 1, -2, 3 };
  auto r = get(v.sp());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(1, r.front());
  r.drop_front();
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(3, r.front());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}

TEST(Filter, EmptyInput_InvalidAtFront) {
  vec v = {};
  auto r = get(v.fwd());
  ASSERT_FALSE(r.at_front().valid());
}

TEST(Filter, OneRejectedElement_InvalidAtFront) {
  vec v = { -1 };
  auto r = get(v.fwd());
  ASSERT_FALSE(r.at_front().valid());
}

TEST(Filter, OneAcceptedElement_AtFrontIsElement) {
  vec v = { 1 };
  auto r = get(v.fwd());
  auto p = r.at_front();
  ASSERT_TRUE(p.valid());
  ASSERT_EQ(1, *p);
}

TEST(Filter, RejectedAccepted_AtFrontIsAccepted) {
  vec v = { -1, 2 };
  auto r = get(v.fwd());
  auto p = r.at_front();
  ASSERT_TRUE(p.valid());
  ASSERT_EQ(2, *p);
}

TEST(Filter, RejectedAcceptedAccepted_SetFrontSecondAtFront_OneElementLeft) {
  vec v = { -1, 2, 3 };
  auto r = get(v.fwd());
  auto s = r;
  s.drop_front();
  r.set_front(s.at_front());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(3, r.front());
}

TEST(Filter, OneAcceptedElement_BackIsElement) {
  vec v = { 1 };
  auto r = get(v.bidi());
  ASSERT_EQ(1, r.back());
}

TEST(Filter, AcceptedRejected_BackIsAccepted) {
  vec v = { 1, -2 };
  auto r = get(v.bidi());
  ASSERT_EQ(1, r.back());
}

TEST(Filter, AcceptedRejectedAccepted_DropBack_BackIsFirst) {
  vec v = { 1, -2, 3 };
  auto r = get(v.bidi());
  r.drop_back();
  ASSERT_EQ(1, r.back());
}

TEST(Filter, OneRejectedElement_InvalidAtBack) {
  vec v = { -1 };
  auto r = get(v.bidi());
  ASSERT_FALSE(r.at_back().valid());
}

TEST(Filter, AcceptedAcceptedRejected_AtBackIsSecond) {
  vec v = { 1, 2, -3 };
  auto r = get(v.bidi());
  auto p = r.at_back();
  ASSERT_TRUE(p.valid());
  ASSERT_EQ(2, *p);
}

TEST(Filter, AcceptedRejectedAccepted_SetBackAtFront_OneLeft) {
  vec v = { 1, -2, 3 };
  auto r = get(v.bidi());
  r.set_back(r.at_front());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(1, r.back());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}
