// Copyright (C) 2003, 2004 by BiRC -- Bioinformatics Research Center
//                             University of Aarhus, Denmark
//                             Contact: Thomas Mailund <mailund@birc.dk>

#include "set-traverser.hh"

SetTraverser::~SetTraverser()
{
    if (_delete_poped) delete_on_stack();
}

void
SetTraverser::delete_on_stack()
{
    while (!_stack.empty())
	{
	    delete _stack.top();
	    _stack.pop();
	}
}

void
SetTraverser::post_visit(Leaf &l)
{
    BitSet *bs = new BitSet(_lm.size());
    (*bs)[_lm[l.name()]] = true;
    _stack.push(bs);
}

void
SetTraverser::post_visit(InnerNode &i)
{
    BitSet *bs = new BitSet(_lm.size());
    while (BitSet *ebs = _stack.top())
	{
	    _stack.pop();
	    *bs |= *ebs;
	    if (_delete_poped) delete ebs;
	}
    _stack.pop();
    _stack.push(bs);
}

void
SetTraverser::complete()
{
    if (_delete_poped) delete_on_stack();
}
