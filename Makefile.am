
CXXFLAGS =
CXXFLAGS += -Wall
CXXFLAGS += -g -O2
#CXXFLAGS += -O3 -pg
#CXXFLAGS += -O3 -fexpensive-optimizations
# -DNDEBUG

# misc.
EXTRA_DIST =
EXTRA_DIST += parser.h
EXTRA_DIST += check-sdist.pl

MAINTAINERCLEANFILES =
MAINTAINERCLEANFILES += parser.cc 
MAINTAINERCLEANFILES += parser.h 
MAINTAINERCLEANFILES += lexer.cc

lexer.cc: parser.h

YFLAGS = -d -t -v


# real program stuff
bin_PROGRAMS = sdist

sdist_LDADD = @LEXLIB@
sdist_SOURCES =
sdist_SOURCES += options.hh options.cc
sdist_SOURCES += lexer.ll parser.yy parser.hh
sdist_SOURCES += tree.hh tree.cc visitor.hh visitor.cc
sdist_SOURCES += bitset.hh bitset.cc
sdist_SOURCES += split-set.hh split-set.cc
sdist_SOURCES += label-map.hh label-map.cc
sdist_SOURCES += label-map-visitor.hh
sdist_SOURCES += set-traverser.hh set-traverser.cc
sdist_SOURCES += set-builder.hh set-builder.cc
sdist_SOURCES += set-matcher.hh set-matcher.cc
sdist_SOURCES += split-set-count.hh split-set-count.cc
sdist_SOURCES += sdist.cc


# test programs
noinst_PROGRAMS =
noinst_PROGRAMS += parser-test
noinst_PROGRAMS += visitor-test
noinst_PROGRAMS += bitset-test
noinst_PROGRAMS += split-set-test
noinst_PROGRAMS += label-map-test
noinst_PROGRAMS += label-map-visitor-test
noinst_PROGRAMS += set-builder-test
noinst_PROGRAMS += set-matcher-test


# FIXME: use script to test output
TESTS = 
TESTS += parser-test
TESTS += visitor-test
TESTS += bitset-test
TESTS += split-set-test
TESTS += label-map-test
TESTS += label-map-visitor-test
TESTS += set-builder-test
TESTS += set-matcher-test
TESTS += check-sdist.pl

parser_test_LDADD = @LEXLIB@
parser_test_SOURCES =
parser_test_SOURCES += lexer.ll parser.yy parser.hh
parser_test_SOURCES += tree.hh tree.cc
parser_test_SOURCES += visitor.hh visitor.cc
parser_test_SOURCES += parser-test.cc

visitor_test_LDADD = @LEXLIB@
visitor_test_SOURCES =
visitor_test_SOURCES += lexer.ll parser.yy parser.hh
visitor_test_SOURCES += tree.hh tree.cc
visitor_test_SOURCES += visitor.hh visitor.cc
visitor_test_SOURCES += visitor-test.cc

bitset_test_SOURCES =
bitset_test_SOURCES += bitset.hh bitset.cc
bitset_test_SOURCES += bitset-test.cc

split_set_test_SOURCES =
split_set_test_SOURCES += bitset.hh bitset.cc
split_set_test_SOURCES += label-map.hh label-map.cc
split_set_test_SOURCES += split-set.hh split-set.cc
split_set_test_SOURCES += split-set-test.cc

label_map_test_SOURCES =
label_map_test_SOURCES += label-map.hh label-map.cc
label_map_test_SOURCES += label-map-test.cc

label_map_visitor_test_LDADD = @LEXLIB@
label_map_visitor_test_SOURCES =
label_map_visitor_test_SOURCES += lexer.ll parser.yy parser.hh
label_map_visitor_test_SOURCES += tree.hh tree.cc
label_map_visitor_test_SOURCES += visitor.hh visitor.cc
label_map_visitor_test_SOURCES += label-map.hh label-map.cc
label_map_visitor_test_SOURCES += label-map-visitor.hh 
label_map_visitor_test_SOURCES += label-map-visitor-test.cc

set_builder_test_LDADD = @LEXLIB@
set_builder_test_SOURCES =
set_builder_test_SOURCES += lexer.ll parser.yy parser.hh
set_builder_test_SOURCES += tree.hh tree.cc visitor.hh visitor.cc
set_builder_test_SOURCES += bitset.hh bitset.cc
set_builder_test_SOURCES += label-map.hh label-map.cc
set_builder_test_SOURCES += label-map-visitor.hh 
set_builder_test_SOURCES += set-traverser.hh set-traverser.cc
set_builder_test_SOURCES += split-set.hh split-set.cc 
set_builder_test_SOURCES += split-set-count.hh split-set-count.cc 
set_builder_test_SOURCES += set-builder.hh set-builder.cc
set_builder_test_SOURCES += set-builder-test.cc

set_matcher_test_LDADD = @LEXLIB@
set_matcher_test_SOURCES =
set_matcher_test_SOURCES += lexer.ll parser.yy parser.hh
set_matcher_test_SOURCES += tree.hh tree.cc visitor.hh visitor.cc
set_matcher_test_SOURCES += bitset.hh bitset.cc
set_matcher_test_SOURCES += label-map.hh label-map.cc
set_matcher_test_SOURCES += label-map-visitor.hh 
set_matcher_test_SOURCES += set-traverser.hh set-traverser.cc
set_matcher_test_SOURCES += set-builder.hh set-builder.cc
set_matcher_test_SOURCES += split-set.hh split-set.cc 
set_matcher_test_SOURCES += split-set-count.hh split-set-count.cc
set_matcher_test_SOURCES += options.hh options.cc
set_matcher_test_SOURCES += set-matcher.hh set-matcher.cc
set_matcher_test_SOURCES += set-matcher-test.cc
