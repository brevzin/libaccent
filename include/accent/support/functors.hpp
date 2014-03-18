#ifndef LIBACCENT_SUPPORT_FUNCTORS_HPP
#define LIBACCENT_SUPPORT_FUNCTORS_HPP

namespace accent { namespace support { namespace functors {

  struct empty_op {
    template <typename Range>
    bool operator ()(const Range& r) const { return r.empty(); }
  };

  struct front_op {
    template <typename Range>
    decltype(auto) operator ()(const Range& r) const { return r.front(); }
  };

  struct drop_front_op {
    template <typename Range>
    void operator ()(Range& r) const { r.drop_front(); }
  };

  struct valid_op {
    template <typename Position>
    bool operator ()(const Position& p) const { return p.valid(); }
  };

  struct deref_op {
    template <typename Position>
    decltype(auto) operator ()(const Position& p) const { return *p; }
  };

}}}

#endif
