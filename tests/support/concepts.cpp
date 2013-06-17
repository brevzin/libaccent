#include <accent/support/concepts.hpp>

// These are sanity checks for the concepts.
using namespace accent::support;

// --------------------- SinglePassRange --------------------------------------

static_assert(!SinglePassRange<int>(),
              "int is misidentified as single pass range");

struct notbool {};

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

struct sp_missing_empty {
  using value_type = int;
  using traversal = accent::single_pass_traversal_tag;

  explicit operator bool() const;
  value_type front() const;
  void drop_front();
};
static_assert(!SinglePassRange<sp_missing_empty>(),
    "single pass concept cannot identify missing empty()");

struct sp_nonconst_empty {
  using value_type = int;
  using traversal = accent::single_pass_traversal_tag;

  bool empty();
  explicit operator bool() const;
  value_type front() const;
  void drop_front();
};
static_assert(!SinglePassRange<sp_nonconst_empty>(),
    "single pass concept cannot identify non-const empty()");

struct sp_garbage_empty {
  using value_type = int;
  using traversal = accent::single_pass_traversal_tag;

  notbool empty() const;
  explicit operator bool() const;
  value_type front() const;
  void drop_front();
};

struct sp_missing_value_type {
  using traversal = accent::single_pass_traversal_tag;

  bool empty() const;
  explicit operator bool() const;
  int front() const;
  void drop_front();
};
static_assert(!SinglePassRange<sp_missing_value_type>(),
    "single pass concept cannot identify missing position");

struct sp_missing_front {
  using value_type = int;
  using traversal = accent::single_pass_traversal_tag;

  bool empty() const;
  explicit operator bool() const;
  void drop_front();
};
static_assert(!SinglePassRange<sp_missing_front>(),
    "single pass concept cannot identify missing front()");

struct sp_nonconst_front {
  using value_type = int;
  using traversal = accent::single_pass_traversal_tag;

  bool empty() const;
  explicit operator bool() const;
  value_type front();
  void drop_front();
};
static_assert(!SinglePassRange<sp_nonconst_front>(),
    "single pass concept cannot identify non-const front()");

struct sp_missing_drop_front {
  using value_type = int;
  using traversal = accent::single_pass_traversal_tag;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
};
static_assert(!SinglePassRange<sp_missing_drop_front>(),
    "single pass concept cannot identify missing drop_front");


// --------------------- ForwardRange -----------------------------------------

struct pos_archetype {
  using value_type = int;

  bool valid() const;
  explicit operator bool() const;
  bool operator !() const;
  value_type operator *() const;
};
bool operator ==(pos_archetype, pos_archetype);
bool operator !=(pos_archetype, pos_archetype);

template <int I>
struct fwd_archetype_i {
  using value_type = int;
  using traversal = accent::forward_traversal_tag;
  using position = pos_archetype;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  void drop_front();

  position at_front() const;
  fwd_archetype_i<I> from(position) const;
  fwd_archetype_i<1-I> until(position) const;
};
using fwd_archetype = fwd_archetype_i<0>;

static_assert(ForwardRange<fwd_archetype>(),
              "fwd_archetype is misidentified as not a forward range");

struct fwd_missing_position {
  using value_type = int;
  using traversal = accent::forward_traversal_tag;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  void drop_front();

  int at_front() const;
  fwd_missing_position from(int) const;
  fwd_missing_position until(int) const;
};

static_assert(!ForwardRange<fwd_missing_position>(),
              "forward concept cannot identify missing position");

struct fwd_missing_at_front {
  using value_type = int;
  using traversal = accent::forward_traversal_tag;
  using position = pos_archetype;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  void drop_front();

  fwd_missing_at_front from(position) const;
  fwd_missing_at_front until(position) const;
};

static_assert(!ForwardRange<fwd_missing_at_front>(),
              "forward concept cannot identify missing at_front");

struct fwd_nonconst_at_front {
  using value_type = int;
  using traversal = accent::forward_traversal_tag;
  using position = pos_archetype;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  void drop_front();

  position at_front();
  fwd_missing_at_front from(position) const;
  fwd_missing_at_front until(position) const;
};

static_assert(!ForwardRange<fwd_missing_at_front>(),
              "forward concept cannot identify non-const at_front");

struct fwd_badret_at_front {
  using value_type = int;
  using traversal = accent::forward_traversal_tag;
  using position = pos_archetype;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  void drop_front();

  int at_front() const;
  fwd_badret_at_front from(position) const;
  fwd_badret_at_front until(position) const;
};

static_assert(!ForwardRange<fwd_badret_at_front>(),
              "forward concept cannot identify bad at_front return value");

struct fwd_missing_from {
  using value_type = int;
  using traversal = accent::forward_traversal_tag;
  using position = pos_archetype;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  void drop_front();

  position at_front() const;
  fwd_missing_from until(position) const;
};

static_assert(!ForwardRange<fwd_missing_from>(),
              "forward concept cannot identify missing from");

struct fwd_nonconst_from {
  using value_type = int;
  using traversal = accent::forward_traversal_tag;
  using position = pos_archetype;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  void drop_front();

  position at_front() const;
  fwd_nonconst_from from(position);
  fwd_nonconst_from until(position) const;
};

static_assert(!ForwardRange<fwd_nonconst_from>(),
              "forward concept cannot identify non-const from");

struct fwd_bad_arg_from {
  using value_type = int;
  using traversal = accent::forward_traversal_tag;
  using position = pos_archetype;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  void drop_front();

  position at_front() const;
  fwd_bad_arg_from from(int) const;
  fwd_bad_arg_from until(position) const;
};

static_assert(!ForwardRange<fwd_bad_arg_from>(),
              "forward concept cannot identify from not taking position");

struct fwd_bad_ret_from {
  using value_type = int;
  using traversal = accent::forward_traversal_tag;
  using position = pos_archetype;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  void drop_front();

  position at_front() const;
  fwd_archetype from(position) const;
  fwd_bad_ret_from until(position) const;
};

static_assert(!ForwardRange<fwd_bad_ret_from>(),
              "forward concept cannot identify from not returning self");

struct fwd_missing_until {
  using value_type = int;
  using traversal = accent::forward_traversal_tag;
  using position = pos_archetype;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  void drop_front();

  position at_front() const;
  fwd_missing_until from(position) const;
};

static_assert(!ForwardRange<fwd_missing_until>(),
              "forward concept cannot identify missing until");

struct fwd_bad_arg_until {
  using value_type = int;
  using traversal = accent::forward_traversal_tag;
  using position = pos_archetype;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  void drop_front();

  position at_front() const;
  fwd_bad_arg_until from(position) const;
  fwd_bad_arg_until until(int) const;
};

static_assert(!ForwardRange<fwd_bad_arg_until>(),
              "forward concept cannot identify until not taking position");

struct fwd_bad_ret_until {
  using value_type = int;
  using traversal = accent::forward_traversal_tag;
  using position = pos_archetype;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  void drop_front();

  position at_front() const;
  fwd_bad_ret_until from(position) const;
  fwd_missing_until until(position) const;
};

static_assert(!ForwardRange<fwd_bad_ret_until>(),
              "forward concept cannot identify until not returning range");

struct fwd_inconsistent_until_position {
  using value_type = int;
  using traversal = accent::forward_traversal_tag;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  void drop_front();

  struct position {
    bool valid() const;
    explicit operator bool() const;
    bool operator !() const;
    value_type operator *() const;
  };
  position at_front() const;
  fwd_inconsistent_until_position from(position) const;
  fwd_archetype until(position) const;
};
bool operator ==(fwd_inconsistent_until_position::position,
                 fwd_inconsistent_until_position::position);
bool operator !=(fwd_inconsistent_until_position::position,
                 fwd_inconsistent_until_position::position);

static_assert(!ForwardRange<fwd_inconsistent_until_position>(),
              "forward concept cannot identify inconsistent position types");


// --------------------- ReadableRange ----------------------------------------

static_assert(!ReadableRange<int>(),
              "int is misidentified as a readable range");

struct r_archetype {
  typedef int value_type;

  int front() const;
};
static_assert(ReadableRange<r_archetype>(),
              "r_archetype is misidentified as not a readable range");

struct r_not_convertible {
  typedef int* value_type;

  int front() const;
};
static_assert(!ReadableRange<r_not_convertible>(),
              "readable range concept cannot identify unconvertible types");


// --------------------- WriteableRange ---------------------------------------

static_assert(!WriteableRange<int>(),
              "int is misidentified as a writeable range");

struct w_archetype {
  typedef int value_type;

  int& front() const;
};
static_assert(WriteableRange<w_archetype>(),
              "w_archetype is misidentified as not a writeable range");

struct w_not_assignable {
  typedef int value_type;

  int front() const;
};
static_assert(!WriteableRange<w_not_assignable>(),
              "writeable range concept cannot identify unassignable types");
