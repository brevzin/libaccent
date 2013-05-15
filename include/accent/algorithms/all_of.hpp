#ifndef LIBACCENT_ALGORITHMS_ALL_OF_HPP
#define LIBACCENT_ALGORITHMS_ALL_OF_HPP

#include "accent/algorithms/any_of.hpp"
#include "accent/functional/not.hpp"

namespace accent { namespace algorithms {

  template <typename SinglePassRange, typename Predicate>
  bool all_of(SinglePassRange r, Predicate p) {
    return !any_of(r, functional::not_(p));
  }

}}

#endif
