// Copyright (C) 2003, 2004 by BiRC -- Bioinformatics Research Center
//                             University of Aarhus, Denmark
//                             Contact: Thomas Mailund <mailund@birc.dk>

#include "set-builder.hh"
#include "parser.hh"
#include "label-map-visitor.hh"
#include "split-set-count.hh"

#include <map>
#include <cassert>
using namespace std;

int
main(int argc, const char *argv[])
{
    Tree *t = parse_string("('A':0.0, ('B':10.1, 'C':0.1), 'D')");

    LabelMapVisitor lm;
    t->dfs_traverse(lm);
    assert(lm.root_label() == "D");

    SetBuilder sb(lm);
    Leaf *root = t->find_leaf(lm.root_label());
    root->dfs_traverse(sb);

    // numbering: D=0, C=1, B=2, A=3

    BitSet a_edge(4);
    a_edge[0] = false;
    a_edge[1] = false;
    a_edge[2] = false;
    a_edge[3] = true;

    BitSet b_edge(4);
    b_edge[0] = false;
    b_edge[1] = false;
    b_edge[2] = true;
    b_edge[3] = false;

    BitSet c_edge(4);
    c_edge[0] = false;
    c_edge[1] = true;
    c_edge[2] = false;
    c_edge[3] = false;

    BitSet d_edge(4);
    d_edge[0] = false;
    d_edge[1] = true;
    d_edge[2] = true;
    d_edge[3] = true;

    BitSet i_edge(4);
    i_edge[0] = false;
    i_edge[1] = true;
    i_edge[2] = true;
    i_edge[3] = false;

    assert(sb.lookup(&a_edge) != 0);
    assert(sb.lookup(&b_edge) != 0);
    assert(sb.lookup(&c_edge) != 0);
    assert(sb.lookup(&d_edge) != 0);
    assert(sb.lookup(&i_edge) != 0);

    BitSet bs(4);
    assert(sb.lookup(&bs) == 0);

    bs[0] = false;
    bs[1] = false;
    bs[2] = true;
    bs[3] = true;
    assert(sb.lookup(&bs) == 0);

    bs[0] = false;
    bs[1] = true;
    bs[2] = false;
    bs[3] = true;
    assert(sb.lookup(&bs) == 0);

    bs[0] = true;
    bs[1] = false;
    bs[2] = false;
    bs[3] = false;
    assert(sb.lookup(&bs) == 0);

    bs[0] = true;
    bs[1] = false;
    bs[2] = false;
    bs[3] = true;
    assert(sb.lookup(&bs) == 0);

    bs[0] = true;
    bs[1] = false;
    bs[2] = true;
    bs[3] = false;
    assert(sb.lookup(&bs) == 0);

    bs[0] = true;
    bs[1] = false;
    bs[2] = true;
    bs[3] = true;
    assert(sb.lookup(&bs) == 0);

    bs[0] = true;
    bs[1] = true;
    bs[2] = false;
    bs[3] = false;
    assert(sb.lookup(&bs) == 0);

    bs[0] = true;
    bs[1] = true;
    bs[2] = false;
    bs[3] = true;
    assert(sb.lookup(&bs) == 0);

    bs[0] = true;
    bs[1] = true;
    bs[2] = true;
    bs[3] = true;
    assert(sb.lookup(&bs) == 0);


    return 0;

    split_set_count::ss_count_iterator_t i;
    for (i = split_set_count::begin(); i != split_set_count::end(); ++i)
	if (!i->first.is_trivial())
	    cout << i->first << " : " << i->second << endl;
}
