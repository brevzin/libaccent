#include <accent/support/concepts.hpp>

// These are sanity checks for the concepts.
using namespace accent::support;

static_assert(!SinglePassRange<int>(),
              "int is misidentified as single pass range");

struct sp_archetype {
  using value_type = int;
  using traversal = accent::single_pass_traversal_tag;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  void drop_front();
};
static_assert(SinglePassRange<sp_archetype>(),
              "sp_archetype is misidentified as not a single pass range");

struct sp_no_copy_ctor {
  using value_type = int;
  using traversal = accent::single_pass_traversal_tag;

  sp_no_copy_ctor(const sp_no_copy_ctor&) = delete;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  void drop_front();
};
static_assert(!SinglePassRange<sp_no_copy_ctor>(),
    "single pass concept cannot identify missing copy constructor");

struct sp_no_copy_assign {
  using value_type = int;
  using traversal = accent::single_pass_traversal_tag;

  sp_no_copy_assign& operator =(const sp_no_copy_assign&) = delete;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  void drop_front();
};
static_assert(!SinglePassRange<sp_no_copy_assign>(),
    "single pass concept cannot identify missing copy assignment");

struct sp_no_boolean_conversion {
  using value_type = int;
  using traversal = accent::single_pass_traversal_tag;

  bool empty() const;
  value_type front() const;
  void drop_front();
};
static_assert(!SinglePassRange<sp_no_boolean_conversion>(),
    "single pass concept cannot identify missing boolean conversion");

struct sp_nonconst_boolean_conversion {
  using value_type = int;
  using traversal = accent::single_pass_traversal_tag;

  bool empty() const;
  explicit operator bool();
  value_type front() const;
  void drop_front();
};
static_assert(!SinglePassRange<sp_nonconst_boolean_conversion>(),
    "single pass concept cannot identify non-const boolean conversion");

struct sp_missing_traversal {
  using value_type = int;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  void drop_front();
};
static_assert(!SinglePassRange<sp_missing_traversal>(),
    "single pass concept cannot identify missing traversal");

struct sp_garbage_traversal {
  using value_type = int;
  using traversal = int;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  void drop_front();
};
static_assert(!SinglePassRange<sp_garbage_traversal>(),
    "single pass concept cannot identify garbage traversal");
