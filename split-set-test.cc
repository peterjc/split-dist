// Copyright (C) 2003, 2004 by BiRC -- Bioinformatics Research Center
//                             University of Aarhus, Denmark
//                             Contact: Thomas Mailund <mailund@birc.dk>

#include "split-set.hh"
#include <cassert>
#include <sstream>
using namespace std;

int
main(int argc, const char *argv[])
{
    LabelMap lm;

    assert(lm.push("foo") == 0);
    assert(lm.push("bar") == 1);
    assert(lm.push("baz") == 2);

    assert(lm["foo"] == 0);
    assert(lm["bar"] == 1);
    assert(lm["baz"] == 2);

    BitSet bs(3);
    SplitSet ss(lm,bs);
    ss[0] = true; ss[2] = true;

    ostringstream os;
    os << ss;

    assert(os.str() == string("foo baz | bar "));
}
