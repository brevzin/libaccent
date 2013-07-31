#include <accent/support/tag_functions.hpp>

#include <type_traits>

using namespace accent;
using namespace accent::support;
using std::is_same;

struct X {};
struct Y {};

template <typename ValueType, typename Traversal, typename Position>
struct pseudo_range {
  using value_type = ValueType;
  using traversal = Traversal;
  using position = Position;
};

template <typename Traversal>
using t = pseudo_range<X, Traversal, X>;

static_assert(is_same<X, value_type_of<pseudo_range<X, Y, Y>>>::value, "");
static_assert(is_same<X, position_of<pseudo_range<Y, Y, X>>>::value, "");
static_assert(is_same<X, traversal_of<pseudo_range<Y, X, Y>>>::value, "");

static_assert(traversal_supports<single_pass_traversal_tag,
                                 t<single_pass_traversal_tag>
                                >::value,
              "bad traversal support");
static_assert(!traversal_supports<forward_traversal_tag,
                                  t<single_pass_traversal_tag>
                                 >::value,
              "bad traversal support");
static_assert(!traversal_supports<bidirectional_traversal_tag,
                                  t<single_pass_traversal_tag>
                                 >::value,
              "bad traversal support");
static_assert(!traversal_supports<random_access_traversal_tag,
                                  t<single_pass_traversal_tag>
                                 >::value,
              "bad traversal support");

static_assert(traversal_supports<single_pass_traversal_tag,
                                 t<forward_traversal_tag>
                                >::value,
              "bad traversal support");
static_assert(traversal_supports<forward_traversal_tag,
                                 t<forward_traversal_tag>
                                >::value,
              "bad traversal support");
static_assert(!traversal_supports<bidirectional_traversal_tag,
                                  t<forward_traversal_tag>
                                 >::value,
              "bad traversal support");
static_assert(!traversal_supports<random_access_traversal_tag,
                                  t<forward_traversal_tag>
                                 >::value,
              "bad traversal support");

static_assert(traversal_supports<single_pass_traversal_tag,
                                 t<bidirectional_traversal_tag>
                                >::value,
              "bad traversal support");
static_assert(traversal_supports<forward_traversal_tag,
                                 t<bidirectional_traversal_tag>
                                >::value,
              "bad traversal support");
static_assert(traversal_supports<bidirectional_traversal_tag,
                                 t<bidirectional_traversal_tag>
                                >::value,
              "bad traversal support");
static_assert(!traversal_supports<random_access_traversal_tag,
                                  t<bidirectional_traversal_tag>
                                 >::value,
              "bad traversal support");

static_assert(traversal_supports<single_pass_traversal_tag,
                                 t<random_access_traversal_tag>
                                >::value,
              "bad traversal support");
static_assert(traversal_supports<forward_traversal_tag,
                                 t<random_access_traversal_tag>
                                >::value,
              "bad traversal support");
static_assert(traversal_supports<bidirectional_traversal_tag,
                                 t<random_access_traversal_tag>
                                >::value,
              "bad traversal support");
static_assert(traversal_supports<random_access_traversal_tag,
                                 t<random_access_traversal_tag>
                                >::value,
              "bad traversal support");

static_assert(is_same<single_pass_traversal_tag,
                      minimum_tag<single_pass_traversal_tag,
                                  single_pass_traversal_tag>
                     >::value,
              "bad minimum tag deduction");
static_assert(is_same<single_pass_traversal_tag,
                      minimum_tag<single_pass_traversal_tag,
                                  forward_traversal_tag>
                     >::value,
              "bad minimum tag deduction");
static_assert(is_same<single_pass_traversal_tag,
                      minimum_tag<single_pass_traversal_tag,
                                  bidirectional_traversal_tag>
                     >::value,
              "bad minimum tag deduction");
static_assert(is_same<single_pass_traversal_tag,
                      minimum_tag<single_pass_traversal_tag,
                                  random_access_traversal_tag>
                     >::value,
              "bad minimum tag deduction");

static_assert(is_same<single_pass_traversal_tag,
                      minimum_tag<forward_traversal_tag,
                                  single_pass_traversal_tag>
                     >::value,
              "bad minimum tag deduction");
static_assert(is_same<forward_traversal_tag,
                      minimum_tag<forward_traversal_tag,
                                  forward_traversal_tag>
                     >::value,
              "bad minimum tag deduction");
static_assert(is_same<forward_traversal_tag,
                      minimum_tag<forward_traversal_tag,
                                  bidirectional_traversal_tag>
                     >::value,
              "bad minimum tag deduction");
static_assert(is_same<forward_traversal_tag,
                      minimum_tag<forward_traversal_tag,
                                  random_access_traversal_tag>
                     >::value,
              "bad minimum tag deduction");

static_assert(is_same<single_pass_traversal_tag,
                      minimum_tag<bidirectional_traversal_tag,
                                  single_pass_traversal_tag>
                     >::value,
              "bad minimum tag deduction");
static_assert(is_same<forward_traversal_tag,
                      minimum_tag<bidirectional_traversal_tag,
                                  forward_traversal_tag>
                     >::value,
              "bad minimum tag deduction");
static_assert(is_same<bidirectional_traversal_tag,
                      minimum_tag<bidirectional_traversal_tag,
                                  bidirectional_traversal_tag>
                     >::value,
              "bad minimum tag deduction");
static_assert(is_same<bidirectional_traversal_tag,
                      minimum_tag<bidirectional_traversal_tag,
                                  random_access_traversal_tag>
                     >::value,
              "bad minimum tag deduction");

static_assert(is_same<single_pass_traversal_tag,
                      minimum_tag<random_access_traversal_tag,
                                  single_pass_traversal_tag>
                     >::value,
              "bad minimum tag deduction");
static_assert(is_same<forward_traversal_tag,
                      minimum_tag<random_access_traversal_tag,
                                  forward_traversal_tag>
                     >::value,
              "bad minimum tag deduction");
static_assert(is_same<bidirectional_traversal_tag,
                      minimum_tag<random_access_traversal_tag,
                                  bidirectional_traversal_tag>
                     >::value,
              "bad minimum tag deduction");
static_assert(is_same<random_access_traversal_tag,
                      minimum_tag<random_access_traversal_tag,
                                  random_access_traversal_tag>
                     >::value,
              "bad minimum tag deduction");

// Variadic n-minimum

static_assert(is_same<forward_traversal_tag,
                      minimum_tag<forward_traversal_tag>
                     >::value,
              "bad minimum tag deduction");

static_assert(is_same<single_pass_traversal_tag,
                      minimum_tag<random_access_traversal_tag,
                                  forward_traversal_tag,
                                  single_pass_traversal_tag,
                                  forward_traversal_tag,
                                  bidirectional_traversal_tag>
                     >::value,
              "bad minimum tag deduction");
