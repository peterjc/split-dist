// Copyright (C) 2003, 2004 by BiRC -- Bioinformatics Research Center
//                             University of Aarhus, Denmark
//                             Contact: Thomas Mailund <mailund@birc.dk>

#ifndef VISITOR_HH
#define VISITOR_HH

#include "tree.hh"

class Visitor {
public:
    virtual void pre_visit(Leaf &l) = 0;
    virtual void pre_visit(InnerNode &i) = 0;
    virtual void pre_visit(Edge &e) = 0;

    virtual void post_visit(Leaf &l) = 0;
    virtual void post_visit(InnerNode &i) = 0;
    virtual void post_visit(Edge &e) = 0;

    virtual void complete() {}	// called when the traversal is all done
protected:
    virtual ~Visitor();
};

class NodeVisitor : public Visitor {
public:
    virtual void pre_visit(Edge &e) {};
    virtual void post_visit(Edge &e) {};
};
class LeafVisitor : public NodeVisitor {
public:
    virtual void pre_visit(InnerNode &i) {};
    virtual void post_visit(InnerNode &i) {};
};

class EdgeVisitor : public Visitor {
public:
    virtual void pre_visit(Leaf &l) {};
    virtual void pre_visit(InnerNode &i) {};
    virtual void post_visit(Leaf &l) {};
    virtual void post_visit(InnerNode &i) {};
};
    

#endif // VISITOR_HH
