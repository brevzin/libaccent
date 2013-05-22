#ifndef LIBACCENT_ALGORITHMS_NONE_OF_HPP
#define LIBACCENT_ALGORITHMS_NONE_OF_HPP

#include "accent/algorithms/any_of.hpp"

namespace accent { namespace algorithms {

  template <typename ReadableSinglePassRange, typename Predicate>
  bool none_of(ReadableSinglePassRange r, Predicate p) {
    return !any_of(r, p);
  }

}}

#endif
