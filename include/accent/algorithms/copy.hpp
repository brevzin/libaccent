#ifndef LIBACCENT_ALGORITHMS_COPY_HPP
#define LIBACCENT_ALGORITHMS_COPY_HPP

#include "accent/support/concepts.hpp"
#include "accent/support/utility.hpp"
#include <type_traits>
#include <utility>

namespace accent { namespace algorithms {

  namespace detail {

    template <typename InputSinglePassRange, typename OutputSinglePassRange>
    std::pair<InputSinglePassRange, OutputSinglePassRange>
    copy(InputSinglePassRange, OutputSinglePassRange, std::false_type);

    template <typename InputSinglePassRange, typename OutputSinglePassRange>
    std::pair<InputSinglePassRange, OutputSinglePassRange>
    copy(InputSinglePassRange in, OutputSinglePassRange out, std::true_type) {
      for (; in && out; in.drop_front(), out.drop_front()) {
        out.front() = in.front();
      }
      return {in, out};
    }

  }

  template <typename InputSinglePassRange, typename OutputSinglePassRange>
  std::pair<InputSinglePassRange, OutputSinglePassRange>
  copy(InputSinglePassRange in, OutputSinglePassRange out) {
    static_assert(support::SinglePassRange<InputSinglePassRange>(),
                  "copy requires a SinglePassRange for its input");
    static_assert(support::SinglePassRange<OutputSinglePassRange>(),
                  "copy requires a SinglePassRange for its output");
    return detail::copy(in, out,
        support::bool_<support::SinglePassRange<InputSinglePassRange>() &&
                       support::SinglePassRange<OutputSinglePassRange>()>());
  }

}}

#endif
