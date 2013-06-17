#include <accent/ranges/vector_range.hpp>
#include "test.hpp"

typedef std::vector<int> ivec;

static_assert(ForwardRange<iterator_range<ivec::iterator>>(),
              "vector iterator range does not model ForwardRange");
static_assert(ForwardRange<iterator_range<
                  ivec::const_iterator>>(),
              "vector const iterator range does not model ForwardRange");

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

TEST(VectorRange, EmptyVector_InvalidAtFront) {
  ivec v = {};
  auto r = adapt(v).all();
  auto p = r.at_front();
  ASSERT_FALSE(p.valid());
}

TEST(VectorRange, OneElementVector_AtFrontIsElement) {
  ivec v = { 1 };
  auto r = adapt(v).all();
  auto p = r.at_front();
  ASSERT_TRUE(p.valid());
  ASSERT_EQ(1, *p);
}

TEST(VectorRange, TwoElementVector_DropFront_AtFrontIsSecondElement) {
  ivec v = { 1, 2 };
  auto r = adapt(v).all();
  r.drop_front();
  auto p = r.at_front();
  ASSERT_TRUE(p.valid());
  ASSERT_EQ(2, *p);
}

TEST(VectorRange, ThreeElementVector_FromSecondElement_ContainsLastTwo) {
  ivec v = { 1, 2, 3 };
  auto r = adapt(v).all();
  auto s = r;
  s.drop_front();
  r = r.from(s.at_front());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(2, r.front());
  r.drop_front();
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(3, r.front());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}

TEST(VectorRange, ThreeElementVector_UntilSecondElement_ContainsFirst) {
  ivec v = { 1, 2, 3 };
  auto r = adapt(v).all();
  auto s = r;
  s.drop_front();
  r = r.until(s.at_front());
  ASSERT_FALSE(r.empty());
  ASSERT_EQ(1, r.front());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}
