#ifndef LIBACCENT_SUPPORT_RANGE_BASE_HPP
#define LIBACCENT_SUPPORT_RANGE_BASE_HPP

namespace accent { namespace support {

  template <typename Derived>
  class range_base
  {
    const Derived& self() const { return *static_cast<const Derived*>(this); }

  public:
    explicit operator bool() const { return !!*this; }
    bool operator !() const { return self().empty(); }
  };

}}

#endif
