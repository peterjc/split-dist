// Copyright (C) 2003 by BiRC -- Bioinformatics Research Center
//                               University of Aarhus, Denmark
//                               Contact: Thomas Mailund <mailund@birc.dk>

#include "parser.hh"
#include "label-map-visitor.hh"
#include "set-builder.hh"
#include "set-matcher.hh"

#include <sstream>
using namespace std;

int
main()
{
    Tree *t = parse_string("('A':0.0, ('B':10.1, 'C':0.1), 'D')");

    ostringstream os1;
    os1 << *t << endl;
    assert(os1.str() == string("('D'0:0, ('C'0:0.1, 'B'0:10.1)0:0, 'A'0:0)\n"));

    LabelMapVisitor lm;
    t->dfs_traverse(lm);
    assert(dynamic_cast<Leaf*>(lm.root())->name() == "D");

    SetBuilder sb(lm);
    lm.root()->dfs_traverse(sb);

    SetMatcher sm(lm,sb);
    lm.root()->dfs_traverse(sm);

    ostringstream os2;
    os2 << *t << endl;
    assert(os2.str() == string("('D'1:0, ('C'1:0.1, 'B'1:10.1)1:0, 'A'1:0)\n"));

    Tree *t1 = parse_string("('A':0.0, ('B':10.1, 'C':0.1), 'D')");
    Tree *t2 = parse_string("(('A':0.0, 'B':10.1):1, 'C':0.1, 'D')");

    LabelMapVisitor lm2;
    t1->dfs_traverse(lm2);
    SetBuilder sb2(lm2);
    SetMatcher sm2(lm2,sb2);

    Leaf *r1 = lm2.root();
    Leaf *r2 = t2->find_leaf(r1->name());

    assert(r1 != 0);
    assert(r2 != 0);

    r1->dfs_traverse(sb2);
    r2->dfs_traverse(sm2);

    ostringstream os3;
    os3 << *t1 << endl;
    assert(os3.str() == string("('D'1:0, ('C'1:0.1, 'B'1:10.1)0:0, 'A'1:0)\n"));
    
}
