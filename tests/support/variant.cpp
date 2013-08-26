#include <accent/support/variant.hpp>
#include <gtest/gtest.h>
using namespace accent::support;

namespace d = accent::support::detail;
static_assert(d::index<int, int, double>() == 0, "bad index: int");
static_assert(d::index<double, int, double>() == 1, "bad index: double");

struct nothrow_move {
  nothrow_move(nothrow_move&&) noexcept(true) {}
  nothrow_move& operator =(nothrow_move&&) noexcept(true) { return *this; }
};
using nothrow_move_variant = variant<int, nothrow_move>;
static_assert(std::is_nothrow_move_constructible<nothrow_move_variant>::value,
              "variant throws on move");
static_assert(std::is_nothrow_move_assignable<nothrow_move_variant>::value,
              "variant throws on move");

struct throwing_move {
  throwing_move(throwing_move&&) noexcept(false) {}
  throwing_move& operator =(throwing_move&&) noexcept(false) { return *this; }
};
using throwing_move_variant = variant<int, throwing_move>;
static_assert(!std::is_nothrow_move_constructible<throwing_move_variant>::value,
              "variant does not throw on move");
static_assert(!std::is_nothrow_move_assignable<throwing_move_variant>::value,
              "variant does not throw on move");

TEST(Variant, IntDouble_DefaultsToInt) {
  variant<int, double> v;
  ASSERT_EQ(0, v.which());
}

struct nodefault {
  nodefault(int) {}
};
TEST(Variant, NodefaultInt_DefaultsToInt) {
  variant<nodefault, int> v;
  ASSERT_EQ(1, v.which());
}

struct throwing {
  throwing() noexcept(false) {}
};
TEST(Variant, ThrowingInt_DefaultsToInt) {
  variant<throwing, int> v;
  ASSERT_EQ(1, v.which());
}

TEST(Variant, IntDouble_IntToInt) {
  variant<int, double> v(1);
  ASSERT_EQ(0, v.which());
}

TEST(Variant, IntDouble_DoubleToDouble) {
  variant<int, double> v(1.0);
  ASSERT_EQ(1, v.which());
}

TEST(Variant, IntDouble_IndexGetter_IntValuePreserved) {
  variant<int, double> v(42);
  ASSERT_EQ(42, v.get<0>());
}

TEST(Variant, IntDouble_IndexGetter_DoubleValuePreserved) {
  variant<int, double> v(3.14);
  ASSERT_EQ(3.14, v.get<1>());
}

TEST(Variant, IntDouble_TypeGetter_IntValuePreserved) {
  variant<int, double> v(42);
  ASSERT_EQ(42, v.get<int>());
};

TEST(Variant, IntDouble_TypeGetter_DoubleValuePreserved) {
  variant<int, double> v(3.14);
  ASSERT_EQ(3.14, v.get<double>());
}

struct dtor_trace {
  int& count;
  ~dtor_trace() { ++count; }
};
TEST(Variant, MemberGetsDestroyed) {
  int destroyed = 0;
  {
    variant<dtor_trace, int> v{dtor_trace{destroyed}};
    destroyed = 0;
  }
  ASSERT_EQ(1, destroyed);
};

struct copy_trace {
  int& count;
  copy_trace(int& count) : count(count) {}
  copy_trace(const copy_trace& o) : count(o.count) { ++count; }
  copy_trace& operator =(const copy_trace&) { return *this; }
};
TEST(Variant, MemberGetsCopied) {
  int copied = 0;
  variant<copy_trace, int> v{copy_trace{copied}};
  copied = 0;
  auto v2 = v;
  ASSERT_EQ(0, v2.which());
  ASSERT_EQ(1, copied);
}

struct move_trace {
  int& count;
  move_trace(int& count) : count(count) {}
  move_trace(move_trace&& o) : count(o.count) { ++count; }
  move_trace& operator =(move_trace&& o) { return *this; }
};
TEST(Variant, MemberGetsMoved) {
  int moved = 0;
  variant<move_trace, int> v{move_trace{moved}};
  moved = 0;
  auto v2 = std::move(v);
  ASSERT_EQ(0, v2.which());
  ASSERT_EQ(1, moved);
}

TEST(Variant, CopyAssignOverDifferent_CopyCtor) {
  int copied = 0;
  variant<copy_trace, int> v{copy_trace{copied}};
  copied = 0;
  variant<copy_trace, int> v2{1};
  v2 = v;
  ASSERT_EQ(0, v2.which());
  ASSERT_EQ(1, copied);
}

struct copy_assign_trace {
  int& count;
  copy_assign_trace& operator =(const copy_assign_trace& o) {
    ++count;
    return *this;
  }
};
TEST(Variant, CopyAssignOverSame_CopyAssign) {
  int copied = 0;
  variant<copy_assign_trace, int> v{copy_assign_trace{copied}};
  variant<copy_assign_trace, int> v2{copy_assign_trace{copied}};
  copied = 0;
  v2 = v;
  ASSERT_EQ(0, v2.which());
  ASSERT_EQ(1, copied);
}

TEST(Variant, MoveAssignOverDifferent_MoveCtor) {
  int moved = 0;
  variant<move_trace, int> v{move_trace{moved}};
  moved = 0;
  variant<move_trace, int> v2{1};
  v2 = std::move(v);
  ASSERT_EQ(0, v2.which());
  ASSERT_EQ(1, moved);
}

struct move_assign_trace {
  int& count;
  move_assign_trace(int& count) : count(count) {}
  move_assign_trace(move_assign_trace&& o) : count(o.count) {}
  move_assign_trace& operator =(move_assign_trace&& o) {
    ++count;
    return *this;
  }
};
TEST(Variant, MoveAssignOverSame_MoveAssign) {
  int moved = 0;
  variant<move_assign_trace, int> v{move_assign_trace{moved}};
  variant<move_assign_trace, int> v2{move_assign_trace{moved}};
  moved = 0;
  v2 = std::move(v);
  ASSERT_EQ(0, v2.which());
  ASSERT_EQ(1, moved);
}

struct get_int {
  int operator ()(int i) { return i == 42 ? 0 : -1; }
  int operator ()(double d) { return d == 3.14 ? 1 : -1; }
};

TEST(Variant, IntDouble_IntApplyToIntWorks) {
  variant<int, double> v{42};
  ASSERT_EQ(0, v.apply(get_int()));
}

TEST(Variant, IntDouble_IntApplyToDoubleWorks) {
  variant<int, double> v{3.14};
  ASSERT_EQ(1, v.apply(get_int()));
}

struct set_int {
  int& result;
  void operator ()(int i) { result = i == 42 ? 0 : -1; }
  void operator ()(double d) { result = d == 3.14 ? 1 : -1; }
};

TEST(Variant, IntDouble_VoidApplyToIntWorks) {
  variant<int, double> v{42};
  int result;
  v.apply(set_int{result});
  ASSERT_EQ(0, result);
}

TEST(Variant, IntDouble_VoidApplyToDoubleWorks) {
  variant<int, double> v{3.14};
  int result;
  v.apply(set_int{result});
  ASSERT_EQ(1, result);
}

struct copy_throws {
  const bool& throw_next;
  copy_throws(const bool& throw_next) : throw_next(throw_next) {}
  copy_throws(const copy_throws& o)
      : throw_next(o.throw_next) {
    if (throw_next) throw 0;
  }
  copy_throws& operator =(const copy_throws& o) { return *this; }
};
TEST(Variant, CopyingThrows1_RevertsToDefault) {
  bool throw_next = true;
  variant<int, copy_throws> v{42};
  try {
    v = copy_throws(throw_next);
    FAIL();
  } catch (int) {}
  ASSERT_EQ(0, v.which());
  ASSERT_EQ(0, v.get<0>());
}

TEST(Variant, CopyingThrows2_RevertsToDefault) {
  bool throw_next = false;
  variant<int, copy_throws> v{42};
  variant<int, copy_throws> v2{copy_throws(throw_next)};
  try {
    throw_next = true;
    v = v2;
    FAIL();
  } catch (int) {}
  ASSERT_EQ(0, v.which());
  ASSERT_EQ(0, v.get<0>());
}

struct move_throws {
  const bool& throw_next;
  move_throws(const bool& throw_next) : throw_next(throw_next) {}
  move_throws(move_throws&& o)
      : throw_next(o.throw_next) {
    if (throw_next) throw 0;
  }
  move_throws& operator =(move_throws&& o) { return *this; }
};
TEST(Variant, MovingThrows_RevertsToDefault) {
  bool throw_next = false;
  variant<int, move_throws> v{42};
  variant<int, move_throws> v2{move_throws(throw_next)};
  try {
    throw_next = true;
    v = std::move(v2);
    FAIL();
  } catch (int) {}
  ASSERT_EQ(0, v.which());
  ASSERT_EQ(0, v.get<0>());
}

TEST(Variant, LargeVariant) {
  variant<char, signed char, unsigned char, signed short, unsigned short, // 5
          signed int, unsigned int, signed long, unsigned long, // 9
          signed long long, unsigned long long, float, double, long double, //14
          void*> v{'a'};
  ASSERT_EQ(0, v.which());
  v = 42;
  ASSERT_EQ(5, v.which());
  v = 3.14;
  ASSERT_EQ(12, v.which());
}

TEST(Variant, ExplicitIndex) {
  variant<int, int> v1(42, 0);
  ASSERT_EQ(0, v1.which());
  ASSERT_EQ(42, v1.get<0>());

  variant<int, int> v2(42, 1);
  ASSERT_EQ(1, v2.which());
  ASSERT_EQ(42, v2.get<1>());
}
