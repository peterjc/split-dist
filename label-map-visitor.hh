// Copyright (C) 2003, 2004 by BiRC -- Bioinformatics Research Center
//                             University of Aarhus, Denmark
//                             Contact: Thomas Mailund <mailund@birc.dk>

#ifndef LABEL_MAP_VISITOR_HH
#define LABEL_MAP_VISITOR_HH

#include "visitor.hh"
#include "label-map.hh"

class LabelMapVisitor : public LeafVisitor, public LabelMap {
    std::string _root_label;
public:
    LabelMapVisitor() {}
    virtual ~LabelMapVisitor() {}
    void pre_visit(Leaf &l)  { /*dummy*/ }
    void post_visit(Leaf &l) {if (push(l.name()) == 0) _root_label = l.name();}

    std::string root_label() const { return _root_label; }
};

#endif // LABEL_MAP_VISITOR_HH

