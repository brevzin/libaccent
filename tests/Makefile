
gtestdir = /Users/wasti/Documents/tests/gtest-1.6.0
boostdir = /Users/wasti/Documents/boost

# GCC 4.6 settings
#CC = g++-4.6
#CXX = g++-4.6
#CXXFLAGS = -I.. -I$(gtestdir)/include \
#  -std=c++0x -Werror -Wall -pedantic -Wno-unused-variable -g -O0
#LDFLAGS = -L$(gtestdir)/libgcc -g -O0
#LDLIBS = -lgtest -lgtest_main

# Clang settings
CC = /usr/local/bin/clang++
CXX = /usr/local/bin/clang++
CXXFLAGS = -std=c++11 -I../include -I$(gtestdir)/include \
  -I$(boostdir) -stdlib=libc++ -U__STRICT_ANSI__ -DGTEST_USE_OWN_TR1_TUPLE \
  -g -O0 -Werror
LDFLAGS = -stdlib=libc++ -g -O0 -L$(gtestdir)/libclang
LDLIBS = -lgtest_main -lgtest -lc++abi

algo_test_suite = any_of all_of none_of

range_test_suite = vector_range

support_test_suite = concepts

test_suites = $(support_test_suite) $(range_test_suite) $(algo_test_suite)

run : test
	./test

test : test.o $(test_suites:=.o)

%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) -MD -o $@ $<
	@cp $*.d $*.P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	      -e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $*.P; \
	  rm -f $*.d

-include $(test_suites:=.P)

clean:
	-rm test *.o *.P

.PHONY: clean run
