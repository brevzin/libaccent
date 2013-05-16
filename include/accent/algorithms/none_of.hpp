#ifndef LIBACCENT_ALGORITHMS_NONE_OF_HPP
#define LIBACCENT_ALGORITHMS_NONE_OF_HPP

#include "accent/algorithms/any_of.hpp"

namespace accent { namespace algorithms {

  template <typename SinglePassRange, typename Predicate>
  bool none_of(SinglePassRange r, Predicate p) {
    return !any_of(r, p);
  }

}}

#endif
