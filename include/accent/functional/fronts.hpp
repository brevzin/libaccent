#ifndef LIBACCENT_FUNCTIONAL_FRONTS_HPP
#define LIBACCENT_FUNCTIONAL_FRONTS_HPP

#include <utility>

namespace accent { namespace functional {

  template <typename Inner>
  class fronts_t {
    Inner inner;

  public:
    explicit fronts_t(Inner inner) : inner(std::move(inner)) {}

    template <typename... Args>
    decltype(auto) operator ()(const Args&... args) const {
      return inner(args.front()...);
    }

    template <typename... Args>
    decltype(auto) operator ()(const Args&... args) {
      return inner(args.front()...);
    }

    Inner base() const { return inner; }
  };

  template <typename Inner>
  fronts_t<Inner> fronts(Inner inner) {
    return fronts_t<Inner>{ std::move(inner) };
  }

}}

#endif
