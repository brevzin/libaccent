#ifndef LIBACCENT_TESTS_TEST_HPP
#define LIBACCENT_TESTS_TEST_HPP

#include <accent/ranges/as.hpp>
#include <accent/ranges/vector_range.hpp>
#include <accent/support/concepts.hpp>
#include <gtest/gtest.h>
#include <initializer_list>
#include <vector>

namespace accent {
  namespace algorithms {}
  namespace ranges {}
  namespace support {}
}

using namespace accent;
using namespace accent::algorithms;
using namespace accent::ranges;
using namespace accent::support;

class vec {
  std::vector<int> v;

public:
  vec() {}
  vec(std::initializer_list<int> l) : v(l) {}

  auto sp() const -> decltype(as_single_pass(adapt(v).all())) {
    return as_single_pass(adapt(v).all());
  }
};

struct positive {
  bool operator ()(int i) const { return i > 0; }
};

struct tracking_positive {
  int& tracker;
  tracking_positive(int& tracker) : tracker(tracker) {}
  bool operator ()(int i) const { ++tracker; return i > 0; }
};

#endif
