#ifndef LIBACCENT_ALGORITHMS_NONE_OF_HPP
#define LIBACCENT_ALGORITHMS_NONE_OF_HPP

#include "accent/algorithms/drop_conditional.hpp"

namespace accent { namespace algorithms {

  template <typename ReadableSinglePassRange, typename Predicate>
  bool none_of_r(ReadableSinglePassRange r, Predicate p) {
    return drop_until_r(r, p).empty();
  }

  template <typename ReadableSinglePassRange, typename Predicate>
  bool none_of(ReadableSinglePassRange r, Predicate p) {
    return none_of_r(r, functional::fronts(p));
  }

}}

#endif
