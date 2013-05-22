#ifndef LIBACCENT_RANGES_PROJECT_HPP
#define LIBACCENT_RANGES_PROJECT_HPP

#include "accent/support/concepts.hpp"
#include "accent/support/range_base.hpp"
#include <boost/compressed_pair.hpp>
#include <type_traits>
#include <utility>

namespace accent { namespace ranges {

  namespace detail {

    template <typename Inner, typename Projection>
    class projected_range :
        public support::range_base<projected_range<Inner, Projection>> {
      boost::compressed_pair<Inner, Projection> data;

      const Inner& inner() const { return data.first(); }
      Inner& inner() { return data.first(); }

      const Projection& projection() const { return data.second(); }

    public:
      using value_type =
          typename std::decay<decltype(std::declval<Projection>()(
                                           std::declval<Inner>().front()))
                             >::type;
      using traversal = support::traversal_of<Inner>;

      projected_range(Inner inner, Projection projection)
        : data(std::move(inner), std::move(projection)) {}

      bool empty() const { return inner().empty(); }

      void drop_front() { inner().drop_front(); }

      auto front() const -> decltype(projection()(inner().front())) {
        return projection()(inner().front());
      }
    };

  }

  template <typename Inner, typename Projection>
  detail::projected_range<Inner, Projection> project(Inner i, Projection p) {
    return {std::move(i), std::move(p)};
  }

}}

#endif
