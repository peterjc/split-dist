// Copyright (C) 2003, 2004 by BiRC -- Bioinformatics Research Center
//                             University of Aarhus, Denmark
//                             Contact: Thomas Mailund <mailund@birc.dk>

#ifndef RESULT_COUNTER_HH
#define RESULT_COUNTER_HH

#include "visitor.hh"

struct ResultCounter : public EdgeVisitor {
    int edge_count;
    int sup_count;
    ResultCounter() : edge_count(0), sup_count(0) {}
    
    virtual void pre_visit(Edge &e) {};
    virtual void post_visit(Edge &e) {
	edge_count++;
	if (e.supported()) sup_count++;
    };
};

struct ClearSupported : EdgeVisitor {
    ClearSupported() {}
    virtual void pre_visit(Edge &e) {};
    virtual void post_visit(Edge &e) { e.reset_supported(); };
};    

#endif // RESULT_COUNTER_HH
