// Copyright (C) 2003, 2004 by BiRC -- Bioinformatics Research Center
//                             University of Aarhus, Denmark
//                             Contact: Thomas Mailund <mailund@birc.dk>

#include "set-builder.hh"
#include "split-set-count.hh"
using split_set_count::inc_split_set_count;

SetBuilder::~SetBuilder()
{
    for (em_t::iterator i = _em.begin(); i != _em.end(); ++i)
	delete i->first;
}

Edge *
SetBuilder::lookup(BitSet *bs) const
{
    em_t::const_iterator i = _em.find(bs);
    return (i != _em.end()) ? i->second : 0;
}



void
SetBuilder::handle_edge(BitSet *bs, Edge *e)
{
    _em[bs] = e;
    inc_split_set_count(SplitSet(_lm,*bs));
}
