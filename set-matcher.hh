// Copyright (C) 2003, 2004 by BiRC -- Bioinformatics Research Center
//                             University of Aarhus, Denmark
//                             Contact: Thomas Mailund <mailund@birc.dk>

#ifndef SET_MATCHER_HH
#define SET_MATCHER_HH

#include "set-traverser.hh"
#include "set-builder.hh"

class SetMatcher : public SetTraverser {
    SetBuilder   &_map;

public:
    SetMatcher(LabelMap &lm, SetBuilder &sb)
	: SetTraverser(lm,true), _map(sb)
    {}
    virtual ~SetMatcher() {}

    virtual void handle_edge(BitSet *bs, Edge *) {
	Edge *split_edge = _map.lookup(bs);
	if (split_edge) split_edge->tag_supported();
    }
};

#endif // SET_MATCHER_HH
