#include <accent/ranges/vector_range.hpp>
#include "test.hpp"

typedef std::vector<int> ivec;

static_assert(SinglePassRange<iterator_range<ivec::iterator>>(),
              "vector iterator range does not model SinglePassRange");
static_assert(SinglePassRange<iterator_range<
                  ivec::const_iterator>>(),
              "vector const iterator range does not model SinglePassRange");

TEST(VectorRange, EmptyConstVector_Empty) {
  const ivec v = {};
  auto r = adapt(v).all();
  ASSERT_TRUE(r.empty());
}

TEST(VectorRange, EmptyVector_Empty) {
  ivec v = {};
  auto r = adapt(v).all();
  ASSERT_TRUE(r.empty());
}

TEST(VectorRange, OneElementVector_NotEmpty) {
  ivec v = { 1 };
  auto r = adapt(v).all();
  ASSERT_FALSE(r.empty());
}

TEST(VectorRange, OneElementVector_FrontIsFirstElement) {
  ivec v = { 1 };
  auto r = adapt(v).all();
  ASSERT_EQ(1, r.front());
}

TEST(VectorRange, OneElementVector_DropFront_Empty) {
  ivec v = { 1 };
  auto r = adapt(v).all();
  r.drop_front();
  ASSERT_TRUE(r.empty());
}

TEST(VectorRange, TwoElementVector_DropFront_NotEmpty) {
  ivec v = { 1, 2 };
  auto r = adapt(v).all();
  r.drop_front();
  ASSERT_FALSE(r.empty());
}

TEST(VectorRange, TwoElementVector_DropFront_FrontIsSecondElement) {
  ivec v = { 1, 2 };
  auto r = adapt(v).all();
  r.drop_front();
  ASSERT_EQ(2, r.front());
}

TEST(VectorRange, TwoElementVector_DropFront_DropFront_Empty) {
  ivec v = { 1, 2 };
  auto r = adapt(v).all();
  r.drop_front();
  r.drop_front();
  ASSERT_TRUE(r.empty());
}
