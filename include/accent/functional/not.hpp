#ifndef LIBACCENT_FUNCTIONAL_NOT_HPP
#define LIBACCENT_FUNCTIONAL_NOT_HPP

#include <utility>

namespace accent { namespace functional {

  template <typename Inner>
  class not_t
  {
    Inner inner;

  public:
    not_t(Inner inner) : inner(std::move(inner)) {}

    template <typename... Args>
    auto operator ()(Args&&... args) const
      -> decltype(!inner(std::forward<Args>(args)...))
    {
      return !inner(std::forward<Args>(args)...);
    }
  };

  template <typename Inner>
  not_t<Inner> not_(Inner inner)
  {
    return { std::move(inner) };
  }

}}

#endif
