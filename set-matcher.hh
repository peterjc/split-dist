// Copyright (C) 2003 by BiRC -- Bioinformatics Research Center
//                               University of Aarhus, Denmark
//                               Contact: Thomas Mailund <mailund@birc.dk>

#ifndef SET_MATCHER_HH
#define SET_MATCHER_HH

#include "set-traverser.hh"
#include "set-builder.hh"

class SetMatcher : public SetTraverser {
    SetBuilder   &_map;
    unsigned int  _edge_count;
    unsigned int  _match_count;

public:
    SetMatcher(LabelMap &lm, SetBuilder &sb)
	: SetTraverser(lm,false), _map(sb), _edge_count(0), _match_count(0)
    {}
    virtual ~SetMatcher() {}

    virtual void handle_edge(BitSet *bs, Edge *) {
	_edge_count++;
	Edge *split_edge = _map.lookup(bs);
	if (split_edge) {
	    split_edge->tag_supported();
	    _match_count++;
	}
    }

    unsigned int edge_count() const  { return _edge_count;  }
    unsigned int match_count() const { return _match_count; }
};

#endif // SET_MATCHER_HH
