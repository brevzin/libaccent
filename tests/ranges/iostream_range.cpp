#include <accent/ranges/iostream_range.hpp>
#include "test.hpp"

typedef std::istream_iterator<int> i_iterator;

static_assert(SinglePassRange<iterator_range<i_iterator>>(),
              "istream iterator range does not model SinglePassRange");
static_assert(!ForwardRange<iterator_range<i_iterator>>(),
              "istream iterator range pretends to model ForwardRange");
