// Copyright (C) 2003 by BiRC -- Bioinformatics Research Center
//                               University of Aarhus, Denmark
//                               Contact: Thomas Mailund <mailund@birc.dk>

#ifndef LABEL_MAP_VISITOR_HH
#define LABEL_MAP_VISITOR_HH

#include "visitor.hh"
#include "label-map.hh"

class LabelMapVisitor : public LeafVisitor, public LabelMap {
    Leaf *_root;
public:
    LabelMapVisitor() : _root(0) {}
    virtual ~LabelMapVisitor()   {}
    void pre_visit(Leaf &l)  { /*dummy*/ }
    void post_visit(Leaf &l) { if (push(l.name()) == 0) _root = &l; }

    Leaf *root() const { return _root; }
};

#endif // LABEL_MAP_VISITOR_HH

