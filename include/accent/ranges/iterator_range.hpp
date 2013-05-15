#ifndef LIBACCENT_RANGES_ITERATOR_RANGE_HPP
#define LIBACCENT_RANGES_ITERATOR_RANGE_HPP

#include "accent/tags.hpp"
#include "accent/support/range_base.hpp"
#include <cassert>
#include <iterator>

namespace accent { namespace ranges {

  template <typename Iterator>
  class iterator_range : public support::range_base<iterator_range<Iterator>> {
    using traits = std::iterator_traits<Iterator>;
    Iterator first, last;

  public:
    using value_type = typename traits::value_type;
    using traversal = single_pass_traversal_tag;

    iterator_range(Iterator first, Iterator last)
      : first(first), last(last)
    {}

    bool empty() const { return first == last; }
    auto front() const -> decltype(*first) {
      assert(!empty());
      return *first;
    }
    void drop_front() {
      assert(!empty());
      ++first;
    }
  };

  template <typename C>
  class container_adapter {
    C& c;

  public:
    explicit container_adapter(C& c) : c(c) {}

    auto all() const -> iterator_range<decltype(c.begin())> {
      return { c.begin(), c.end() };
    }
  };

}}

#endif
