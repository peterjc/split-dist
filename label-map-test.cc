// Copyright (C) 2003, 2004 by BiRC -- Bioinformatics Research Center
//                             University of Aarhus, Denmark
//                             Contact: Thomas Mailund <mailund@birc.dk>


#include "label-map.hh"
#include <cassert>

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

    try
	{
	    lm.push("foo");
	    assert(false);
	}
    catch (LabelMap::AlreadyPushedEx ex)
	{
	}
    catch(...)
	{
	    assert(false);
	}

    try
	{
	    lm["qux"];
	    assert(false);
	}
    catch (LabelMap::UnkownLabelEx ex)
	{
	}
    catch(...)
	{
	    assert(false);
	}


    
}
