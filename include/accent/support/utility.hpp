#ifndef LIBACCENT_SUPPORT_UTILITY_HPP
#define LIBACCENT_SUPPORT_UTILITY_HPP

#include <type_traits>

namespace accent { namespace support {

  template <bool V>
  using bool_ = std::integral_constant<bool, V>;

}}

#endif
