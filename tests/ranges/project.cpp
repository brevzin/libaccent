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
using sp_s_range = ranges::detail::as_single_pass_range<s_range>;
using i_of_sp_s_range = projected_range<sp_s_range, grab_i>;

static_assert(ForwardRange<i_of_s_range>(),
              "projected forward range does not model ForwardRange");
static_assert(SinglePassRange<i_of_sp_s_range>(),
              "projected single pass range does not model SinglePassRange");
static_assert(!ForwardRange<i_of_sp_s_range>(),
              "projected single pass range claims to model ForwardRange");

TEST(SpProject, emptySource_emptyProjected) {
  svec v = {};
  auto r = project(as_single_pass(adapt(v).all()), grab_i());
  ASSERT_TRUE(r.empty());
}

TEST(FwdProject, emptySource_emptyProjected) {
  svec v = {};
  auto r = project(adapt(v).all(), grab_i());
  ASSERT_TRUE(r.empty());
}

TEST(SpProject, oneElementSource_notEmptyProjected) {
  svec v = { {1} };
  auto r = project(as_single_pass(adapt(v).all()), grab_i());
  ASSERT_FALSE(r.empty());
}

TEST(FwdProject, oneElementSource_notEmptyProjected) {
  svec v = { {1} };
  auto r = project(adapt(v).all(), grab_i());
  ASSERT_FALSE(r.empty());
}

TEST(SpProject, elementRead_projectedRead) {
  svec v = { {1} };
  auto r = project(as_single_pass(adapt(v).all()), grab_i());
  ASSERT_EQ(1, r.front());
}

TEST(FwdProject, elementRead_projectedRead) {
  svec v = { {1} };
  auto r = project(adapt(v).all(), grab_i());
  ASSERT_EQ(1, r.front());
}

TEST(SpProject, projectedWrite_elementWrite) {
  svec v = { {1} };
  auto r = project(as_single_pass(adapt(v).all()), grab_i());
  r.front() = 2;
  ASSERT_EQ(2, v.front().i);
}

TEST(FwdProject, projectedWrite_elementWrite) {
  svec v = { {1} };
  auto r = project(adapt(v).all(), grab_i());
  r.front() = 2;
  ASSERT_EQ(2, v.front().i);
}

TEST(SpProject, oneElementSource_dropFront_emptyProjected) {
  svec v = { {1} };
  auto r = project(as_single_pass(adapt(v).all()), grab_i());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}

TEST(FwdProject, oneElementSource_dropFront_emptyProjected) {
  svec v = { {1} };
  auto r = project(adapt(v).all(), grab_i());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}

TEST(FwdProject, emptySource_atFrontIsInvalid) {
  svec v = {};
  auto r = project(adapt(v).all(), grab_i());
  auto p = r.at_front();
  ASSERT_FALSE(p.valid());
}

TEST(FwdProject, oneElementSource_atFrontIsElement) {
  svec v = { {1} };
  auto r = project(adapt(v).all(), grab_i());
  auto p = r.at_front();
  ASSERT_TRUE(p.valid());
  ASSERT_EQ(1, *p);
}


TEST(FwdProject, twoElementSource_dropFront_atFrontIsSecondElement) {
  svec v = { {1}, {2} };
  auto r = project(adapt(v).all(), grab_i());
  r.drop_front();
  auto p = r.at_front();
  ASSERT_TRUE(p.valid());
  ASSERT_EQ(2, *p);
}

TEST(FwdProject, threeElementSource_setFrontSecondElement_containsLastTwo) {
  svec v = { {1}, {2}, {3} };
  auto r = project(adapt(v).all(), grab_i());
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
#if 0
TEST(FwdProject, threeElementSource_untilSecondElement_containsFirst) {
  svec v = { {1}, {2}, {3} };
  auto r = project(adapt(v).all(), grab_i());
  auto s = r;
  s.drop_front();
  r = r.until(s.at_front());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(1, r.front());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}
#endif