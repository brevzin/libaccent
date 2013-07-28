#ifndef LIBACCENT_ALGORITHMS_ALL_OF_HPP
#define LIBACCENT_ALGORITHMS_ALL_OF_HPP

#include "accent/algorithms/none_of.hpp"
#include "accent/functional/not.hpp"

namespace accent { namespace algorithms {

  template <typename ReadableSinglePassRange, typename Predicate>
  bool all_of_r(ReadableSinglePassRange r, Predicate p) {
    return none_of_r(r, functional::not_(p));
  }

  template <typename ReadableSinglePassRange, typename Predicate>
  bool all_of(ReadableSinglePassRange r, Predicate p) {
    return all_of_r(r, functional::fronts(p));
  }

}}

#endif
