#ifndef LIBACCENT_RANGES_APPEND_HPP
#define LIBACCENT_RANGES_APPEND_HPP

#include "accent/tags.hpp"
#include "accent/support/range_base.hpp"
#include <utility>

namespace accent { namespace ranges {

  template <typename C>
  class appender : public support::range_base<appender<C>> {
    C* c;

  public:
    using value_type = typename C::value_type;
    using traversal = single_pass_traversal_tag;

    explicit appender(C& c) : c(&c) {}

    bool empty() const { return c->size() == c->max_size(); }
    appender front() const { return *this; }
    void drop_front() {}

    appender& operator =(const value_type& v) {
      c->push_back(v);
      return *this;
    }

    appender& operator =(value_type&& v) {
      c->push_back(std::move(v));
      return *this;
    }
  };

  template <typename C>
  appender<C> append(C& c) {
    return appender<C>(c);
  }

}}

#endif
