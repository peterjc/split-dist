// Copyright (C) 2003, 2004 by BiRC -- Bioinformatics Research Center
//                             University of Aarhus, Denmark
//                             Contact: Thomas Mailund <mailund@birc.dk>

#include "label-map-visitor.hh"
#include "tree.hh"
#include "parser.hh"

int
main(int argc, const char *argv[])
{
    Tree *t = parse_string("('A':0.0, ('B':10.1, 'C':0.1), 'D')");
    LabelMapVisitor lm;

    t->dfs_traverse(lm);

    assert(lm.size() == 4);

    assert(lm["D"] == 0);
    assert(lm["C"] == 1);
    assert(lm["B"] == 2);
    assert(lm["A"] == 3);

    assert(lm.root_label() == "D");
    assert(lm.root_label() == t->find_leaf("D")->name());
}
