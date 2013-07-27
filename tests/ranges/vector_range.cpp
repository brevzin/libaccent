#include <accent/ranges/vector_range.hpp>
#include "test.hpp"

typedef std::vector<int> ivec;

static_assert(BidirectionalRange<iterator_range<ivec::iterator>>(),
              "vector iterator range does not model BidirectionalRange");
static_assert(BidirectionalRange<iterator_range<
                  ivec::const_iterator>>(),
              "vector const iterator range does not model BidirectionalRange");

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

TEST(VectorRange, OneElementVector_DropFront_InvalidAtFront) {
  ivec v = { 1 };
  auto r = adapt(v).all();
  r.drop_front();
  auto p = r.at_front();
  ASSERT_FALSE(p.valid());
}

TEST(VectorRange, TwoElementVector_DropFront_AtFrontIsSecondElement) {
  ivec v = { 1, 2 };
  auto r = adapt(v).all();
  r.drop_front();
  auto p = r.at_front();
  ASSERT_TRUE(p.valid());
  ASSERT_EQ(2, *p);
}

TEST(VectorRange, ThreeElementVector_SetFrontSecondElement_ContainsLastTwo) {
  ivec v = { 1, 2, 3 };
  auto r = adapt(v).all();
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

TEST(VectorRange, OneElementVector_BackIsFirstElement) {
  ivec v = { 1 };
  auto r = adapt(v).all();
  ASSERT_EQ(1, r.back());
}

TEST(VectorRange, OneElementVector_DropBack_Empty) {
  ivec v = { 1 };
  auto r = adapt(v).all();
  r.drop_back();
  ASSERT_TRUE(r.empty());
}

TEST(VectorRange, TwoElementVector_DropBack_NotEmpty) {
  ivec v = { 1, 2 };
  auto r = adapt(v).all();
  r.drop_back();
  ASSERT_FALSE(r.empty());
}

TEST(VectorRange, TwoElementVector_BackIsSecondElement) {
  ivec v = { 1, 2 };
  auto r = adapt(v).all();
  ASSERT_EQ(2, r.back());
}

TEST(VectorRange, TwoElementVector_DropBack_BackIsFirstElement) {
  ivec v = { 1, 2 };
  auto r = adapt(v).all();
  r.drop_back();
  ASSERT_EQ(1, r.back());
}

TEST(VectorRange, TwoElementVector_DropBack_DropBack_Empty) {
  ivec v = { 1, 2 };
  auto r = adapt(v).all();
  r.drop_back();
  r.drop_back();
  ASSERT_TRUE(r.empty());
}

TEST(VectorRange, TwoElementVector_DropFront_DropBack_Empty) {
  ivec v = { 1, 2 };
  auto r = adapt(v).all();
  r.drop_front();
  r.drop_back();
  ASSERT_TRUE(r.empty());
}

TEST(VectorRange, TwoElementVector_DropBack_DropFront_Empty) {
  ivec v = { 1, 2 };
  auto r = adapt(v).all();
  r.drop_back();
  r.drop_front();
  ASSERT_TRUE(r.empty());
}

TEST(VectorRange, EmptyVector_InvalidAtBack) {
  ivec v = {};
  auto r = adapt(v).all();
  auto p = r.at_back();
  ASSERT_FALSE(p.valid());
}

TEST(VectorRange, OneElementVector_AtBackIsElement) {
  ivec v = { 1 };
  auto r = adapt(v).all();
  auto p = r.at_back();
  ASSERT_TRUE(p.valid());
  ASSERT_EQ(1, *p);
}

TEST(VectorRange, OneElementVector_DropBack_InvalidAtBack) {
  ivec v = { 1 };
  auto r = adapt(v).all();
  r.drop_front();
  auto p = r.at_back();
  ASSERT_FALSE(p.valid());
}

TEST(VectorRange, TwoElementVector_AtBackIsSecondElement) {
  ivec v = { 1, 2 };
  auto r = adapt(v).all();
  auto p = r.at_back();
  ASSERT_TRUE(p.valid());
  ASSERT_EQ(2, *p);
}

TEST(VectorRange, TwoElementVector_DropBack_AtBackIsFirstElement) {
  ivec v = { 1, 2 };
  auto r = adapt(v).all();
  r.drop_back();
  auto p = r.at_back();
  ASSERT_TRUE(p.valid());
  ASSERT_EQ(1, *p);
}

TEST(VectorRange, SetBackAtBack_SameElements) {
  ivec v = { 1, 2 };
  auto r = adapt(v).all();
  r.set_back(r.at_back());
  ASSERT_FALSE(r.empty());
  r.drop_front();
  ASSERT_FALSE(r.empty());
  r.drop_front();
  ASSERT_TRUE(r.empty());
}

TEST(VectorRange, ThreeElementVector_SetBackSecondElement_ContainsFirstTwo) {
  ivec v = { 1, 2, 3 };
  auto r = adapt(v).all();
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
