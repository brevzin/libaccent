#ifndef LIBACCENT_ALGORITHMS_ANY_OF_HPP
#define LIBACCENT_ALGORITHMS_ANY_OF_HPP

#include "accent/algorithms/drop_conditional.hpp"

namespace accent { namespace algorithms {

  template <typename ReadableSinglePassRange, typename Predicate>
  bool any_of(ReadableSinglePassRange r, Predicate p) {
    return !drop_until(r, p).empty();
  }

}}

#endif
