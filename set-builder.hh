// Copyright (C) 2003, 2004 by BiRC -- Bioinformatics Research Center
//                             University of Aarhus, Denmark
//                             Contact: Thomas Mailund <mailund@birc.dk>

#ifndef SET_BUILDER_HH
#define SET_BUILDER_HH

#include "set-traverser.hh"

#include <map>

class SetBuilder : public SetTraverser {
    struct BSCmp : public std::binary_function<const BitSet*,const BitSet*,bool>{
	bool operator()(const BitSet *b1, const BitSet *b2) const {
	    return *b1 < *b2;
	}
    };

public:
    typedef std::map<BitSet*,Edge*,BSCmp> em_t;

private:
    em_t _em;

public:
    SetBuilder(LabelMap &lm) : SetTraverser(lm,false) {}
    virtual ~SetBuilder();

    Edge *lookup(BitSet *bs) const;

    virtual void handle_edge(BitSet *bs, Edge *e);
};

#endif // SET_BUILDER_HH
