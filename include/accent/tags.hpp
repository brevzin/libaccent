#ifndef LIBACCENT_TAGS_HPP
#define LIBACCENT_TAGS_HPP

namespace accent
{

  struct single_pass_traversal_tag {};
  struct forward_traversal_tag : single_pass_traversal_tag {};
  struct bidirectional_traversal_tag : forward_traversal_tag {};
  struct random_access_traversal_tag : bidirectional_traversal_tag {};

}

#endif
