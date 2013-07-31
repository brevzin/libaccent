#include <accent/ranges/project.hpp>
#include "test.hpp"

struct S {
  int i;
};

typedef std::vector<int> ivec;
typedef std::vector<S> svec;

struct grab_i {
  int& operator ()(S& s) const { return s.i; }
  const int& operator ()(const S& s) const { return s.i; }
};

using s_range = iterator_range<svec::iterator>;
using i_of_s_range = projected_range<s_range, grab_i>;
using fwd_s_range = ranges::detail::as_forward_range<s_range>;
using i_of_fwd_s_range = projected_range<fwd_s_range, grab_i>;
using sp_s_range = ranges::detail::as_single_pass_range<s_range>;
using i_of_sp_s_range = projected_range<sp_s_range, grab_i>;

static_assert(BidirectionalRange<i_of_s_range>(),
             "projected bidirectional range does not model BidirectionalRange");
static_assert(ForwardRange<i_of_fwd_s_range>(),
              "projected forward range does not model ForwardRange");
static_assert(!BidirectionalRange<i_of_fwd_s_range>(),
              "projected forward range claims to model BidirectionalRange");
static_assert(SinglePassRange<i_of_sp_s_range>(),
              "projected single pass range does not model SinglePassRange");
static_assert(!ForwardRange<i_of_sp_s_range>(),
              "projected single pass range claims to model ForwardRange");

i_of_sp_s_range sp_i(svec& v) {
  return project(as_single_pass(adapt(v).all()), grab_i());
}

i_of_fwd_s_range fwd_i(svec& v) {
  return project(as_forward(adapt(v).all()), grab_i());
}

i_of_s_range bidi_i(svec& v) {
  return project(adapt(v).all(), grab_i());
}

TEST(Project, emptySource_emptyProjected) {
  svec v = {};
  auto r = project(as_single_pass(adapt(v).all()), grab_i());
  ASSERT_TRUE(r.empty());
}

TEST(Project, oneElementSource_notEmptyProjected) {
  svec v = { {1} };
  auto r = sp_i(v);
  ASSERT_FALSE(r.empty());
}

TEST(Project, elementRead_projectedRead) {
  svec v = { {1} };
  auto r = sp_i(v);
  ASSERT_EQ(1, r.front());
}

TEST(Project, projectedWrite_elementWrite) {
  svec v = { {1} };
  auto r = sp_i(v);
  r.front() = 2;
  ASSERT_EQ(2, v.front().i);
}

TEST(Project, oneElementSource_dropFront_emptyProjected) {
  svec v = { {1} };
  auto r = sp_i(v);
  r.drop_front();
  ASSERT_TRUE(r.empty());
}

TEST(Project, twoElementSource_dropFront_frontIsSecond) {
  svec v = { {1}, {2} };
  auto r = sp_i(v);
  r.drop_front();
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(2, r.front());
}

TEST(Project, emptySource_atFrontIsInvalid) {
  svec v = {};
  auto r = fwd_i(v);
  auto p = r.at_front();
  ASSERT_FALSE(p.valid());
}

TEST(Project, oneElementSource_atFrontIsElement) {
  svec v = { {1} };
  auto r = fwd_i(v);
  auto p = r.at_front();
  ASSERT_TRUE(p.valid());
  ASSERT_EQ(1, *p);
}

TEST(Project, twoElementSource_dropFront_atFrontIsSecondElement) {
  svec v = { {1}, {2} };
  auto r = fwd_i(v);
  r.drop_front();
  auto p = r.at_front();
  ASSERT_TRUE(p.valid());
  ASSERT_EQ(2, *p);
}

TEST(Project, threeElementSource_setFrontSecondElement_containsLastTwo) {
  svec v = { {1}, {2}, {3} };
  auto r = fwd_i(v);
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

TEST(Project, elementReadBack_projectedRead) {
  svec v = { {1} };
  auto r = bidi_i(v);
  ASSERT_EQ(1, r.back());
}

TEST(Project, projectedWriteBack_elementWrite) {
  svec v = { {1} };
  auto r = bidi_i(v);
  r.back() = 2;
  ASSERT_EQ(2, v.back().i);
}

TEST(Project, oneElementSource_dropBack_emptyProjected) {
  svec v = { {1} };
  auto r = bidi_i(v);
  r.drop_back();
  ASSERT_TRUE(r.empty());
}

TEST(Project, twoElementSource_backIsSecond) {
  svec v = { {1}, {2} };
  auto r = bidi_i(v);
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(2, r.back());
}

TEST(Project, twoElementSource_dropBack_backIsFirst) {
  svec v = { {1}, {2} };
  auto r = bidi_i(v);
  r.drop_back();
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(1, r.back());
}

TEST(Project, emptySource_atBackIsInvalid) {
  svec v = {};
  auto r = bidi_i(v);
  auto p = r.at_back();
  ASSERT_FALSE(p.valid());
}

TEST(Project, oneElementSource_atBackIsElement) {
  svec v = { {1} };
  auto r = bidi_i(v);
  auto p = r.at_back();
  ASSERT_TRUE(p.valid());
  ASSERT_EQ(1, *p);
}

TEST(Project, twoElementSource_atBackIsSecondElement) {
  svec v = { {1}, {2} };
  auto r = bidi_i(v);
  auto p = r.at_back();
  ASSERT_TRUE(p.valid());
  ASSERT_EQ(2, *p);
}

TEST(Project, twoElementSource_dropBack_atBackIsFirstElement) {
  svec v = { {1}, {2} };
  auto r = bidi_i(v);
  r.drop_back();
  auto p = r.at_back();
  ASSERT_TRUE(p.valid());
  ASSERT_EQ(1, *p);
}

TEST(Project, threeElementSource_setBackSecondElement_containsFirstTwo) {
  svec v = { {1}, {2}, {3} };
  auto r = bidi_i(v);
  auto s = r;
  s.drop_front();
  r.set_back(s.at_front());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(1, r.front());
  r.drop_front();
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(2, r.front());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}
