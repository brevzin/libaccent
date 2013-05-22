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

static_assert(SinglePassRange<ranges::detail::projected_range<
                                  iterator_range<svec::iterator>, grab_i>>(),
              "projected single pass range does not model SinglePassRange");

TEST(Project, emptySource_emptyProjected) {
  svec v = {};
  auto r = project(adapt(v).all(), grab_i());
  ASSERT_TRUE(r.empty());
}

TEST(Project, oneElementSource_notEmptyProjected) {
  svec v = { {1} };
  auto r = project(adapt(v).all(), grab_i());
  ASSERT_FALSE(r.empty());
}

TEST(Project, elementRead_projectedRead) {
  svec v = { {1} };
  auto r = project(adapt(v).all(), grab_i());
  ASSERT_EQ(1, r.front());
}

TEST(Project, projectedWrite_elementWrite) {
  svec v = { {1} };
  auto r = project(adapt(v).all(), grab_i());
  r.front() = 2;
  ASSERT_EQ(2, v.front().i);
}

TEST(Project, oneElementSource_dropFront_emptyProjected) {
  svec v = { {1} };
  auto r = project(adapt(v).all(), grab_i());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}
