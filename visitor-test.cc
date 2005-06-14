// Copyright (C) 2003, 2004 by BiRC -- Bioinformatics Research Center
//                             University of Aarhus, Denmark
//                             Contact: Thomas Mailund <mailund@birc.dk>

#include "parser.hh"
#include "visitor.hh"

#include <set>
#include <map>
#include <string>
#include <cassert>
using namespace std;

class LabelTester : public LeafVisitor {
    set<string> &_labels;
public:
    LabelTester(set<string> &labels) : _labels(labels) {};
    virtual void pre_visit(Leaf &l) {/*dummy*/}
    virtual void post_visit(Leaf &l);
};


void LabelTester::post_visit(Leaf &l)
{
    assert(_labels.count(l.name()) == 1);
    _labels.erase(l.name());
}



class LeafCollector : public LeafVisitor {
    map<string,Node*> &_m;
public:
    LeafCollector(map<string,Node*> &m) : _m(m) {};
    virtual void pre_visit(Leaf &l) {/*dummy*/}
    virtual void post_visit(Leaf &l);
};


void LeafCollector::post_visit(Leaf &l)
{
    _m[l.name()] = &l;
}




template <class T>
void
test_traversal(T *t, set<string> labels)
{
    LabelTester lt(labels);
    t->dfs_traverse(lt);
    assert(labels.empty());
}


int
main(int argc, const char *argv[])
{
    Tree *t = parse_string("('A':0.0, ('B':10.1, 'C':0.1), 'D')");

    set<string> labels;
    labels.insert("A");
    labels.insert("B");
    labels.insert("C");
    labels.insert("D");

    test_traversal(t,labels);

    map<string,Node*> leaf_map;
    LeafCollector lc(leaf_map);
    t->dfs_traverse(lc);

    assert(leaf_map.size() == 4);

    test_traversal(leaf_map["A"], labels);
    test_traversal(leaf_map["B"], labels);
    test_traversal(leaf_map["C"], labels);
    test_traversal(leaf_map["D"], labels);
}
