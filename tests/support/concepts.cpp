#include <accent/support/concepts.hpp>

// These are sanity checks for the concepts.
using namespace accent::support;

struct notbool {};

// --------------------- Position ---------------------------------------------

static_assert(!Position<int>(), "int misidentified as a position");

struct pos_archetype {
  using value_type = int;

  bool valid() const;
  explicit operator bool() const;
  value_type operator *() const;
};
bool operator ==(pos_archetype, pos_archetype);
bool operator !=(pos_archetype, pos_archetype);

static_assert(Position<pos_archetype>(),
              "pos_archetype misidentified as not a position");

struct pos_missing_value_type {
  bool valid() const;
  explicit operator bool() const;
  int operator *() const;
};
bool operator ==(pos_missing_value_type, pos_missing_value_type);
bool operator !=(pos_missing_value_type, pos_missing_value_type);

static_assert(!Position<pos_missing_value_type>(),
              "position concept cannot identify missing value_type");

struct pos_missing_valid {
  using value_type = int;

  explicit operator bool() const;
  value_type operator *() const;
};
bool operator ==(pos_missing_valid, pos_missing_valid);
bool operator !=(pos_missing_valid, pos_missing_valid);

static_assert(!Position<pos_missing_valid>(),
              "position concept cannot identify missing valid()");

struct pos_nonconst_valid {
  using value_type = int;

  bool valid();
  explicit operator bool() const;
  value_type operator *() const;
};
bool operator ==(pos_nonconst_valid, pos_nonconst_valid);
bool operator !=(pos_nonconst_valid, pos_nonconst_valid);

static_assert(!Position<pos_nonconst_valid>(),
              "position concept cannot identify non-const valid()");

struct pos_garbage_valid {
  using value_type = int;

  notbool valid() const;
  explicit operator bool() const;
  value_type operator *() const;
};
bool operator ==(pos_garbage_valid, pos_garbage_valid);
bool operator !=(pos_garbage_valid, pos_garbage_valid);

static_assert(!Position<pos_garbage_valid>(),
              "position concept cannot identify garbage valid()");

struct pos_no_default_ctor {
  using value_type = int;

  pos_no_default_ctor(int);

  bool valid() const;
  explicit operator bool() const;
  value_type operator *() const;
};
bool operator ==(pos_no_default_ctor, pos_no_default_ctor);
bool operator !=(pos_no_default_ctor, pos_no_default_ctor);

static_assert(!Position<pos_no_default_ctor>(),
              "position concept cannot identify missing default constructor");

struct pos_no_copy_ctor {
  using value_type = int;

  pos_no_copy_ctor(const pos_no_copy_ctor&) = delete;

  bool valid() const;
  explicit operator bool() const;
  value_type operator *() const;
};
bool operator ==(pos_no_copy_ctor, pos_no_copy_ctor);
bool operator !=(pos_no_copy_ctor, pos_no_copy_ctor);

static_assert(!Position<pos_no_copy_ctor>(),
              "position concept cannot identify missing copy constructor");

struct pos_no_copy_assign {
  using value_type = int;

  pos_no_copy_assign& operator =(const pos_no_copy_assign&) = delete;

  bool valid() const;
  explicit operator bool() const;
  value_type operator *() const;
};
bool operator ==(pos_no_copy_assign, pos_no_copy_assign);
bool operator !=(pos_no_copy_assign, pos_no_copy_assign);

static_assert(!Position<pos_no_copy_assign>(),
              "position concept cannot identify missing copy assignment");

struct pos_missing_boolean_conversion {
  using value_type = int;

  bool valid() const;
  value_type operator *() const;
};
bool operator ==(pos_missing_boolean_conversion,pos_missing_boolean_conversion);
bool operator !=(pos_missing_boolean_conversion,pos_missing_boolean_conversion);

static_assert(!Position<pos_missing_boolean_conversion>(),
              "position concept cannot identify missing boolean conversion");

struct pos_nonconst_boolean_conversion {
  using value_type = int;

  bool valid() const;
  explicit operator bool();
  value_type operator *() const;
};
bool operator ==(pos_nonconst_boolean_conversion,
                 pos_nonconst_boolean_conversion);
bool operator !=(pos_nonconst_boolean_conversion,
                 pos_nonconst_boolean_conversion);

static_assert(!Position<pos_nonconst_boolean_conversion>(),
              "position concept cannot identify non-const boolean conversion");

struct pos_missing_dereference {
  using value_type = int;

  bool valid() const;
  explicit operator bool() const;
};
bool operator ==(pos_missing_dereference, pos_missing_dereference);
bool operator !=(pos_missing_dereference, pos_missing_dereference);

static_assert(!Position<pos_missing_dereference>(),
              "position concept cannot identify missing dereference");

struct pos_nonconst_dereference {
  using value_type = int;

  bool valid() const;
  explicit operator bool() const;
  value_type operator *();
};
bool operator ==(pos_nonconst_dereference, pos_nonconst_dereference);
bool operator !=(pos_nonconst_dereference, pos_nonconst_dereference);

static_assert(!Position<pos_nonconst_dereference>(),
              "position concept cannot identify non-const dereference");

struct pos_void_dereference {
  using value_type = int;

  bool valid() const;
  explicit operator bool() const;
  void operator *() const;
};
bool operator ==(pos_void_dereference, pos_void_dereference);
bool operator !=(pos_void_dereference, pos_void_dereference);

static_assert(!Position<pos_void_dereference>(),
              "position concept cannot identify void dereference");

struct pos_missing_equality {
  using value_type = int;

  bool valid() const;
  explicit operator bool() const;
  value_type operator *() const;
};
bool operator !=(pos_missing_equality, pos_missing_equality);

static_assert(!Position<pos_missing_equality>(),
              "position concept cannot identify missing equality");

struct pos_nonconst_equality_lhs {
  using value_type = int;

  bool valid() const;
  explicit operator bool() const;
  value_type operator *() const;
};
bool operator ==(pos_nonconst_equality_lhs&, pos_nonconst_equality_lhs);
bool operator !=(pos_nonconst_equality_lhs, pos_nonconst_equality_lhs);

static_assert(!Position<pos_nonconst_equality_lhs>(),
              "position concept cannot identify non-const equality on lhs");

struct pos_nonconst_equality_rhs {
  using value_type = int;

  bool valid() const;
  explicit operator bool() const;
  value_type operator *() const;
};
bool operator ==(pos_nonconst_equality_rhs, pos_nonconst_equality_rhs&);
bool operator !=(pos_nonconst_equality_rhs, pos_nonconst_equality_rhs);

static_assert(!Position<pos_nonconst_equality_rhs>(),
              "position concept cannot identify non-const equality on rhs");

struct pos_garbage_equality {
  using value_type = int;

  bool valid() const;
  explicit operator bool() const;
  value_type operator *() const;
};
notbool operator ==(pos_garbage_equality, pos_garbage_equality);
bool operator !=(pos_garbage_equality, pos_garbage_equality);

static_assert(!Position<pos_garbage_equality>(),
              "position concept cannot identify garbage equality");

struct pos_missing_inequality {
  using value_type = int;

  bool valid() const;
  explicit operator bool() const;
  value_type operator *() const;
};
bool operator ==(pos_missing_inequality, pos_missing_inequality);

static_assert(!Position<pos_missing_inequality>(),
              "position concept cannot identify missing inequality");

struct pos_nonconst_inequality_lhs {
  using value_type = int;

  bool valid() const;
  explicit operator bool() const;
  value_type operator *() const;
};
bool operator ==(pos_nonconst_inequality_lhs, pos_nonconst_inequality_lhs);
bool operator !=(pos_nonconst_inequality_lhs&, pos_nonconst_inequality_lhs);

static_assert(!Position<pos_nonconst_inequality_lhs>(),
              "position concept cannot identify non-const inequality on lhs");

struct pos_nonconst_inequality_rhs {
  using value_type = int;

  bool valid() const;
  explicit operator bool() const;
  value_type operator *() const;
};
bool operator ==(pos_nonconst_inequality_rhs, pos_nonconst_inequality_rhs);
bool operator !=(pos_nonconst_inequality_rhs, pos_nonconst_inequality_rhs&);

static_assert(!Position<pos_nonconst_inequality_rhs>(),
              "position concept cannot identify non-const inequality on rhs");

struct pos_garbage_inequality {
  using value_type = int;

  bool valid() const;
  explicit operator bool() const;
  value_type operator *() const;
};
bool operator ==(pos_garbage_inequality, pos_garbage_inequality);
notbool operator !=(pos_garbage_inequality, pos_garbage_inequality);

static_assert(!Position<pos_garbage_inequality>(),
              "position concept cannot identify garbage inequality");


// --------------------- SinglePassRange --------------------------------------

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
static_assert(!SinglePassRange<sp_garbage_empty>(),
    "single pass concept cannot identify garbage empty()");

struct sp_missing_value_type {
  using traversal = accent::single_pass_traversal_tag;

  bool empty() const;
  explicit operator bool() const;
  int front() const;
  void drop_front();
};
static_assert(!SinglePassRange<sp_missing_value_type>(),
    "single pass concept cannot identify missing value_type");

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

static_assert(!ForwardRange<int>(), "int misidentified as a forward range");

struct fwd_archetype {
  using value_type = int;
  using traversal = accent::forward_traversal_tag;
  using position = pos_archetype;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  void drop_front();

  position at_front() const;
  void set_front(position);
};

static_assert(ForwardRange<fwd_archetype>(),
              "fwd_archetype is misidentified as not a forward range");

struct fwd_insufficient_traversal {
  using value_type = int;
  using traversal = accent::single_pass_traversal_tag;
  using position = pos_archetype;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  void drop_front();

  position at_front() const;
  void set_front(position);
};

static_assert(!ForwardRange<fwd_insufficient_traversal>(),
              "forward concept cannot identify insufficient traversal");

struct fwd_missing_position {
  using value_type = int;
  using traversal = accent::forward_traversal_tag;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  void drop_front();

  int at_front() const;
  void set_front(int);
};

static_assert(!ForwardRange<fwd_missing_position>(),
              "forward concept cannot identify missing position");

struct fwd_incompatible_position {
  using value_type = int;
  using traversal = accent::forward_traversal_tag;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  void drop_front();

  struct position {
    using value_type = double;
    bool valid() const;
    explicit operator bool() const;
    value_type operator *() const;
  };
  position at_front() const;
  void set_front(position);
};
bool operator ==(fwd_incompatible_position::position,
                 fwd_incompatible_position::position);
bool operator !=(fwd_incompatible_position::position,
                 fwd_incompatible_position::position);

static_assert(!ForwardRange<fwd_incompatible_position>(),
              "forward concept cannot identify incompatible position");

struct fwd_missing_at_front {
  using value_type = int;
  using traversal = accent::forward_traversal_tag;
  using position = pos_archetype;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  void drop_front();

  void set_front(position);
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
  void set_front(position);
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
  void set_front(position);
};

static_assert(!ForwardRange<fwd_badret_at_front>(),
              "forward concept cannot identify bad at_front return value");

struct fwd_missing_set_front {
  using value_type = int;
  using traversal = accent::forward_traversal_tag;
  using position = pos_archetype;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  void drop_front();

  position at_front() const;
};

static_assert(!ForwardRange<fwd_missing_set_front>(),
              "forward concept cannot identify missing set_front");

struct fwd_bad_arg_set_front {
  using value_type = int;
  using traversal = accent::forward_traversal_tag;
  using position = pos_archetype;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  void drop_front();

  position at_front() const;
  void set_front(int);
};

static_assert(!ForwardRange<fwd_bad_arg_set_front>(),
              "forward concept cannot identify set_front not taking position");


// --------------------- BidirectionalRange -----------------------------------

struct bidi_archetype {
  using value_type = int;
  using traversal = accent::bidirectional_traversal_tag;
  using position = pos_archetype;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  value_type back() const;
  void drop_front();
  void drop_back();

  position at_front() const;
  position at_back() const;
  void set_front(position);
  void set_back(position);
};

static_assert(BidirectionalRange<bidi_archetype>(),
              "bidi_archetype is misidentified as not a bidirectional range");

struct bidi_insufficient_traversal {
  using value_type = int;
  using traversal = accent::forward_traversal_tag;
  using position = pos_archetype;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  value_type back() const;
  void drop_front();
  void drop_back();

  position at_front() const;
  position at_back() const;
  void set_front(position);
  void set_back(position);
};

static_assert(!BidirectionalRange<bidi_insufficient_traversal>(),
              "bidirectional range concept cannot identify "
              "insufficient traversal");

struct bidi_missing_back {
  using value_type = int;
  using traversal = accent::bidirectional_traversal_tag;
  using position = pos_archetype;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  void drop_front();
  void drop_back();

  position at_front() const;
  position at_back() const;
  void set_front(position);
  void set_back(position);
};

static_assert(!BidirectionalRange<bidi_missing_back>(),
              "bidirectional range concept cannot identify missing back");

struct bidi_inconsistent_back {
  using value_type = int;
  using traversal = accent::bidirectional_traversal_tag;
  using position = pos_archetype;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  long back() const;
  void drop_front();
  void drop_back();

  position at_front() const;
  position at_back() const;
  void set_front(position);
  void set_back(position);
};

static_assert(!BidirectionalRange<bidi_inconsistent_back>(),
              "bidirectional range concept cannot identify "
              "back and front mismatch");

struct bidi_nonconst_back {
  using value_type = int;
  using traversal = accent::bidirectional_traversal_tag;
  using position = pos_archetype;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  value_type back();
  void drop_front();
  void drop_back();

  position at_front() const;
  position at_back() const;
  void set_front(position);
  void set_back(position);
};

static_assert(!BidirectionalRange<bidi_nonconst_back>(),
              "bidirectional range concept cannot identify non-const back");

struct bidi_missing_drop_back {
  using value_type = int;
  using traversal = accent::bidirectional_traversal_tag;
  using position = pos_archetype;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  value_type back() const;
  void drop_front();

  position at_front() const;
  position at_back() const;
  void set_front(position);
  void set_back(position);
};

static_assert(!BidirectionalRange<bidi_missing_drop_back>(),
              "bidirectional range concept cannot identify missing drop_back");

struct bidi_missing_at_back {
  using value_type = int;
  using traversal = accent::bidirectional_traversal_tag;
  using position = pos_archetype;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  value_type back() const;
  void drop_front();
  void drop_back();

  position at_front() const;
  void set_front(position);
  void set_back(position);
};

static_assert(!BidirectionalRange<bidi_missing_at_back>(),
              "bidirectional range concept cannot identify missing at_back");

struct bidi_nonconst_at_back {
  using value_type = int;
  using traversal = accent::bidirectional_traversal_tag;
  using position = pos_archetype;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  value_type back() const;
  void drop_front();
  void drop_back();

  position at_front() const;
  position at_back();
  void set_front(position);
  void set_back(position);
};

static_assert(!BidirectionalRange<bidi_nonconst_at_back>(),
              "bidirectional range concept cannot identify "
              "non-const drop_back");

struct bidi_badret_at_back {
  using value_type = int;
  using traversal = accent::bidirectional_traversal_tag;
  using position = pos_archetype;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  value_type back() const;
  void drop_front();
  void drop_back();

  position at_front() const;
  int at_back() const;
  void set_front(position);
  void set_back(position);
};

static_assert(!BidirectionalRange<bidi_badret_at_back>(),
              "bidirectional range concept cannot identify "
              "bad drop_back return type");

struct bidi_missing_set_back {
  using value_type = int;
  using traversal = accent::bidirectional_traversal_tag;
  using position = pos_archetype;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  value_type back() const;
  void drop_front();
  void drop_back();

  position at_front() const;
  position at_back() const;
  void set_front(position);
};

static_assert(!BidirectionalRange<bidi_missing_set_back>(),
              "bidirectional range concept cannot identify missing set_back");

struct bidi_badarg_set_back {
  using value_type = int;
  using traversal = accent::bidirectional_traversal_tag;
  using position = pos_archetype;

  bool empty() const;
  explicit operator bool() const;
  value_type front() const;
  value_type back() const;
  void drop_front();
  void drop_back();

  position at_front() const;
  position at_back() const;
  void set_front(position);
  void set_back(int);
};

static_assert(!BidirectionalRange<bidi_badarg_set_back>(),
              "bidirectional range concept cannot identify "
              "set_back not taking position");

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
