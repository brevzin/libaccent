#ifndef LIBACCENT_FUNCTIONAL_FRONTS_HPP
#define LIBACCENT_FUNCTIONAL_FRONTS_HPP

#include <utility>

namespace accent { namespace functional {

  template <typename Inner>
  class fronts_t {
    Inner inner;

  public:
    fronts_t(Inner inner) : inner(std::move(inner)) {}

    template <typename... Args>
    auto operator ()(const Args&... args) const
      -> decltype(inner(args.front()...))
    {
      return inner(args.front()...);
    }

    template <typename... Args>
    auto operator ()(const Args&... args)
      -> decltype(inner(args.front()...))
    {
      return inner(args.front()...);
    }

    Inner base() const { return inner; }
  };

  template <typename Inner>
  fronts_t<Inner> fronts(Inner inner) {
    return { std::move(inner) };
  }

}}

#endif
