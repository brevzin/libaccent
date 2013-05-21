#include <accent/ranges/append.hpp>
#include "test.hpp"

typedef std::vector<int> ivec;

static_assert(SinglePassRange<appender<ivec>>(),
              "vector appender does not model SinglePassRange");

TEST(Append, Constructed_NotEmpty) {
  ivec v;
  auto r = append(v);
  ASSERT_FALSE(r.empty());
}

TEST(Append, AssignFront_InVector) {
  ivec v;
  auto r = append(v);
  r.front() = 1;
  ASSERT_EQ(1, v.size());
  ASSERT_EQ(1, v[0]);
}

TEST(Append, AssignFront_DropFront_AssignFront_InVector) {
  ivec v;
  auto r = append(v);
  r.front() = 1;
  r.drop_front();
  r.front() = 2;
  ASSERT_EQ(2, v.size());
  ASSERT_EQ(1, v[0]);
  ASSERT_EQ(2, v[1]);
}
