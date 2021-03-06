#ifndef LIBACCENT_RANGES_ITERATOR_RANGE_HPP
#define LIBACCENT_RANGES_ITERATOR_RANGE_HPP

#include "accent/support/range.hpp"
#include <cassert>
#include <iterator>

namespace accent { namespace ranges {

  namespace detail {
    template <typename IteratorCategory>
    struct iterator_category_to_range_traversal;

    template <>
    struct iterator_category_to_range_traversal<std::input_iterator_tag> {
      using type = single_pass_traversal_tag;
    };

    template <>
    struct iterator_category_to_range_traversal<std::output_iterator_tag> {
      using type = single_pass_traversal_tag;
    };

    template <>
    struct iterator_category_to_range_traversal<std::forward_iterator_tag> {
      using type = forward_traversal_tag;
    };

    template <>
    struct iterator_category_to_range_traversal<std::bidirectional_iterator_tag>
    {
      using type = bidirectional_traversal_tag;
    };

    template <>
    struct iterator_category_to_range_traversal<std::random_access_iterator_tag>
    {
      using type = random_access_traversal_tag;
    };
  }

  template <typename Iterator>
  class iterator_range : public support::range_base<iterator_range<Iterator>> {
    using traits = std::iterator_traits<Iterator>;
    Iterator first, last;

  public:
    using value_type = typename traits::value_type;
    using traversal = typename detail::iterator_category_to_range_traversal<
                          typename traits::iterator_category>::type;

    class position {
      Iterator it;
      bool has_it;
      friend class iterator_range;

      position(Iterator it, bool has_it) : it(it), has_it(has_it) {}

    public:
      using value_type = typename traits::value_type;

      position() : has_it(false) {}

      bool valid() const { return has_it; }
      explicit operator bool() const { return valid(); }

      auto operator *() const -> decltype(*it) {
        assert(valid());
        return *it;
      }

      friend bool operator ==(const position& l, const position& r) {
        if (l && r) return l.it == r.it;
        return l.valid() == r.valid();
      }
      friend bool operator !=(const position& l, const position& r) {
        return !(l == r);
      }
    };

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
    position at_front() const { return { first, !empty() }; }
    void set_front(position p) {
      assert(p);
      // If random-access, should assert first <= p.it < last.
      first = p.it;
    }
    auto back() const -> decltype(*first) {
      assert(!empty());
      auto it = last;
      --it;
      return *it;
    }
    void drop_back() {
      assert(!empty());
      --last;
    }
    position at_back() const {
      if (empty())
        return { first, false };
      auto it = last;
      --it;
      return { it, true };
    }
    void set_back(position p) {
      assert(p);
      // If random-access, should assert first <= p.it < last.
      last = p.it;
      ++last;
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
