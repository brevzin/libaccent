#ifndef LIBACCENT_ALGORITHMS_COPY_HPP
#define LIBACCENT_ALGORITHMS_COPY_HPP

#include "accent/support/concepts.hpp"
#include "accent/support/utility.hpp"
#include <type_traits>
#include <utility>

namespace accent { namespace algorithms {

  namespace detail {

    template <typename ReadableSinglePassRange,
              typename WriteableSinglePassRange>
    std::pair<ReadableSinglePassRange, WriteableSinglePassRange>
    copy(ReadableSinglePassRange, WriteableSinglePassRange, std::false_type);

    template <typename ReadableSinglePassRange,
              typename WriteableSinglePassRange>
    std::pair<ReadableSinglePassRange, WriteableSinglePassRange>
    copy(ReadableSinglePassRange in, WriteableSinglePassRange out,
         std::true_type) {
      for (; in && out; in.drop_front(), out.drop_front()) {
        out.front() = in.front();
      }
      return {in, out};
    }

  }

  template <typename ReadableSinglePassRange, typename WriteableSinglePassRange>
  std::pair<ReadableSinglePassRange, WriteableSinglePassRange>
  copy(ReadableSinglePassRange in, WriteableSinglePassRange out) {
    static_assert(support::SinglePassRange<ReadableSinglePassRange>(),
                  "copy requires a SinglePassRange for its input");
    static_assert(support::ReadableRange<ReadableSinglePassRange>(),
                  "copy requires a ReadableRange for its input");
    static_assert(support::SinglePassRange<WriteableSinglePassRange>(),
                  "copy requires a SinglePassRange for its output");
    return detail::copy(in, out,
        support::bool_<support::SinglePassRange<ReadableSinglePassRange>() &&
                       support::ReadableRange<ReadableSinglePassRange>() &&
                       support::SinglePassRange<WriteableSinglePassRange>()>());
  }

}}

#endif
