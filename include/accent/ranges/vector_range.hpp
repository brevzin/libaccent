#ifndef LIBACCENT_RANGES_VECTOR_RANGE_HPP
#define LIBACCENT_RANGES_VECTOR_RANGE_HPP

#include "accent/ranges/iterator_range.hpp"
#include <vector>

namespace accent { namespace ranges {

  template <typename E, typename Alloc>
  container_adapter<std::vector<E, Alloc>>
      adapt(std::vector<E, Alloc>& v) {
    return container_adapter<std::vector<E, Alloc>>(v);
  }

  template <typename E, typename Alloc>
  container_adapter<const std::vector<E, Alloc>>
      adapt(const std::vector<E, Alloc>& v) {
    return container_adapter<const std::vector<E, Alloc>>(v);
  }

}}

#endif
