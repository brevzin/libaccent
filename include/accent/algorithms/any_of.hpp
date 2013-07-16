#ifndef LIBACCENT_ALGORITHMS_ANY_OF_HPP
#define LIBACCENT_ALGORITHMS_ANY_OF_HPP

#include "accent/algorithms/none_of.hpp"

namespace accent { namespace algorithms {

  template <typename ReadableSinglePassRange, typename Predicate>
  bool any_of(ReadableSinglePassRange r, Predicate p) {
    return !none_of(r, p);
  }

}}

#endif
