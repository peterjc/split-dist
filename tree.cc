// Copyright (C) 2003, 2004 by BiRC -- Bioinformatics Research Center
//                             University of Aarhus, Denmark
//                             Contact: Thomas Mailund <mailund@birc.dk>

#include "tree.hh"
#include "visitor.hh"
#include <functional>
using namespace std;

int Tree::no_trees;

// --- constructors and destructors -----------------------------------
Tree::Tree()  {}
Tree::~Tree() {}

Edge::Edge(Tree *t2, float length)
    : _t1(0), _t2(t2), _length(length),
      _supported_count(1) // the edge itself always support
{
    assert(t2 != 0);
    _t2->connect(this);
}

Leaf::Leaf(const char *name)
    : Tree(), _name(name), _e(0)
{}
void
Leaf::connect(Edge *e)
{
    _e = e;
}


InnerNode::InnerNode(list<Edge*> *edges)
    : Tree()
{
    for_each(edges->begin(),edges->end(),bind2nd(mem_fun(&Edge::set_t1),this));
}
void
InnerNode::connect(Edge *e)
{
    _edges.push_back(e);
}


// --- I/O methods ----------------------------------------------------
void
Edge::print(ostream &os, const class Tree *from) const
{
    Tree *sub_tree = (from == _t1) ? _t2 : _t1;
    sub_tree->print(os,this);
    // make sure we do not divide by zero
    if (Tree::number_of_trees() > 1)
	os << ' '
	   << static_cast<double>(_supported_count)/Tree::number_of_trees()
	   << " : " << _length;
    else
	os << " 1 : " << _length; // if it is the only tree it is full support
}

void
Leaf::print(ostream &os, const class Edge *from) const
{
    if (from == _e)
	os << '\'' << _name << '\''; // print as leaf
    else
	// print using leaf as root...not obvious how to do that
	assert(0);
}

void
InnerNode::print(ostream &os, const class Edge *from) const
{
    const char *sep = "";
    os << '(';
    list<Edge*>::const_iterator i;
    for (i = _edges.begin(); i != _edges.end(); ++i)
	{
	    if (*i != from)
		{
		    os << sep; sep = ", ";
		    (*i)->print(os,this);
		}
	}
    os << ')'; 
}



// --- visitor pattern ------------------------------------------------
void
Tree::dfs_traverse(Visitor &v)
{
    dfs(v,0);
    v.complete();
}

void
Edge::dfs(Visitor &v, const Tree *from)
{
    v.pre_visit(*this);
    if (from == _t1) _t2->dfs(v, this);
    else             _t1->dfs(v, this);
    v.post_visit(*this);
}

void
Leaf::dfs(Visitor &v, const Edge *from)
{
    v.pre_visit(*this);
    if (from != _e) _e->dfs(v, this);
    v.post_visit(*this);
}

namespace {
    class EV : public unary_function<Edge*,void> {
	Visitor    &_v;
	Tree       *_this;
	const Edge *_exclude;
    public:
	EV(Visitor &v, Tree *t, const Edge *ex)
	    : _v(v), _this(t), _exclude(ex) {};
	void operator()(Edge *e) { if (e != _exclude) e->dfs(_v,_this); }
    };
};

void
InnerNode::dfs(Visitor &v, const Edge *from)
{
    v.pre_visit(*this);
    for_each(_edges.begin(), _edges.end(), EV(v,this,from));
    v.post_visit(*this);
}




// --- search for named leaf ------------------------------------------
namespace {
    struct Found {
	Leaf *l;
	Found(Leaf *l) : l(l) {};
    };
    struct SV : public LeafVisitor {
	string name;
	SV(string name) : name(name) {}
	virtual ~SV() {}
	void pre_visit(Leaf &l)  { if (l.name() == name) throw Found(&l); }
	void post_visit(Leaf &l) {}
    };
};
class Leaf *
Tree::find_leaf(string name)
{
    try {
	SV v(name); dfs_traverse(v);
    } catch (Found f) {
	return f.l;
    }
    return 0;
}
