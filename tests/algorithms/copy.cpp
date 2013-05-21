#include <accent/algorithms/copy.hpp>
#include <accent/ranges/append.hpp>
#include "test.hpp"

typedef std::vector<int> ivec;

class CopyFidelity : public ::testing::TestWithParam<vec> {};

TEST_P(CopyFidelity, copyToAppender) {
  vec v = GetParam();
  ivec t;
  copy(v.sp(), append(t));
  ASSERT_EQ(v.get(), t);
}

TEST_P(CopyFidelity, copyToPreallocated) {
  vec v = GetParam();
  ivec t(v.get().size());
  copy(v.sp(), adapt(t).all());
  ASSERT_EQ(v.get(), t);
}

INSTANTIATE_TEST_CASE_P(variousVectors, CopyFidelity,
  ::testing::Values(vec{}, vec{1}, vec{1, 2}, vec{1, 2, 3}));
