// Copyright (C) 2003, 2004 by BiRC -- Bioinformatics Research Center
//                             University of Aarhus, Denmark
//                             Contact: Thomas Mailund <mailund@birc.dk>

#ifndef SET_TRAVERSER_HH
#define SET_TRAVERSER_HH

#include "visitor.hh"
#include "bitset.hh"
#include "label-map.hh"

#include <map>
#include <stack>

class SetTraverser : public Visitor {
    // INVARIANT: when returning to an inner node, the non-null
    // elements at the top of the stack contains the bit-sets for the
    // immediate children of the node.
    std::stack<BitSet*> _stack;
    bool _delete_poped; // flag determining whether pop'ed sets are deleted

    void delete_on_stack();

protected:
    LabelMap &_lm;

public:
    SetTraverser(LabelMap &lm, bool delete_poped)
	: _delete_poped(delete_poped), _lm(lm)
    {}
    virtual ~SetTraverser();

    // visitor
    virtual void pre_visit(Leaf &l)      {}
    virtual void pre_visit(InnerNode &i) { _stack.push(0); }
    virtual void pre_visit(Edge &e)      {}

    virtual void post_visit(Leaf &l);
    virtual void post_visit(InnerNode &i);
    virtual void post_visit(Edge &e) 	 { handle_edge(_stack.top(), &e); }

    virtual void complete();

    // hooks for specializations
    virtual void handle_edge(BitSet *bs, Edge *e) = 0;
};

#endif // SET_TRAVERSER_HH
