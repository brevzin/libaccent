#ifndef LIBACCENT_ALGORITHMS_ANY_OF_HPP
#define LIBACCENT_ALGORITHMS_ANY_OF_HPP

#include "accent/support/concepts.hpp"
#include "accent/support/tag_functions.hpp"
#include "accent/support/utility.hpp"

namespace accent { namespace algorithms {

  namespace detail {

    template <typename SinglePassRange, typename Predicate>
    bool any_of(SinglePassRange, Predicate, std::false_type);

    template <typename SinglePassRange, typename Predicate>
    bool any_of(SinglePassRange r, Predicate p, std::true_type) {
      return false;
    }

  }

  template <typename SinglePassRange, typename Predicate>
  bool any_of(SinglePassRange r, Predicate p) {
    static_assert(support::SinglePassRange<SinglePassRange>(),
                  "any_of requires a SinglePassRange");
    return detail::any_of(r, p,
        support::bool_<support::SinglePassRange<SinglePassRange>()>());
  }

}}

#endif
