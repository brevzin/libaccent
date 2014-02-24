
algo_test_suite = any_of all_of none_of copy drop_conditional find reverse \
    for_each equal rotate

range_test_suite = vector_range append project iostream_range take_conditional \
    filter reversed concat zip

support_test_suite = concepts tag_functions variant

test_suites = $(addprefix support/, $(support_test_suite)) \
              $(addprefix ranges/, $(range_test_suite)) \
              $(addprefix algorithms/, $(algo_test_suite))

include ../../make.rules
