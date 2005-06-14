// Copyright (C) 2003, 2004 by BiRC -- Bioinformatics Research Center
//                             University of Aarhus, Denmark
//                             Contact: Thomas Mailund <mailund@birc.dk>

#include "tree.hh"
#include "visitor.hh"
#include <functional>
#include <cassert>
using namespace std;


// --- constructors and destructors -----------------------------------
Node::Node()  {}
Node::~Node() {}

Edge::Edge(Node *t2, double length)
    : _t1(0), _t2(t2), _length(length),
      _supported_count(1) // the edge itself always support
{
    assert(t2 != 0);
    _t2->connect(this);
}

Leaf::Leaf(const char *name)
    : Node(), _name(name), _e(0)
{}
void
Leaf::connect(Edge *e)
{
    _e = e;
}


InnerNode::InnerNode(list<Edge*> *edges)
    : Node()
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
Edge::print(ostream &os, const class Node *from) const
{
    Node *sub_tree = (from == _t1) ? _t2 : _t1;
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
Node::dfs_traverse(Visitor &v)
{
    dfs(v,0);
    v.complete();
}

void
Edge::dfs(Visitor &v, const Node *from)
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
	Node       *_this;
	const Edge *_exclude;
    public:
	EV(Visitor &v, Node *t, const Edge *ex)
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
Node::find_leaf(string name)
{
    try {
	SV v(name); dfs_traverse(v);
    } catch (Found f) {
	return f.l;
    }
    return 0;
}


int Tree::no_trees;

#include <vector>
namespace {
    struct NodeFound {
	Node *n;
	NodeFound(Node *n) : n(n) {}
    };

    struct FindDeg2NodeVisitor : public NodeVisitor {
	virtual void pre_visit(Leaf &l) {}
	virtual void post_visit(Leaf &l) {}
	virtual void pre_visit(InnerNode &i) {}
	virtual void post_visit(InnerNode &i) {
	    //assert(i.no_edges() > 1);
	    if (i.no_edges() == 2) throw NodeFound(&i);
	}
    };

    struct FindHighDegreeVisitor : public NodeVisitor {
	virtual void pre_visit(Leaf &l) {}
	virtual void post_visit(Leaf &l) {}

	void check_node(InnerNode &n)
	{
	    if (n.no_edges() > 2) throw NodeFound(&n);
	}

	virtual void pre_visit(InnerNode &i)  { check_node(i); }
	virtual void post_visit(InnerNode &i) { check_node(i); }
    };

    InnerNode * find_degree_2_node(Node *n)
    {
	FindDeg2NodeVisitor v;
	try {
	    n->dfs_traverse(v);
	    return 0;
	} catch (NodeFound f) {
	    return (InnerNode*)f.n;
	}
    }

    Node * find_high_degree_node(Node *n)
    {
	FindHighDegreeVisitor v;
	try {
	    n->dfs_traverse(v);
	    throw no_node_with_high_degree();
	} catch (NodeFound f) {
	    return f.n;
	}
    }
}

Tree::Tree(Node *node) : _node(node) 
{
    InnerNode *n;
    while ((n = find_degree_2_node(_node)))
	{
	    if (n == _node)
		_node = find_high_degree_node(_node);

	    std::list<Edge*>::iterator j = n->_edges.begin();
	    Edge *e1 = *j; ++j;
	    Edge *e2 = *j;
	    
	    Node *other1 = (n == e1->t1()) ? e1->t2() : e1->t1();
	    Node *other2 = (n == e2->t1()) ? e2->t2() : e2->t1();

	    // removing n -- FIXME: memory leak here, but since the
	    // trees aren't GC'ed anyway I won't bother with it here
	    // either

	    if (InnerNode *i = dynamic_cast<InnerNode*>(other1))
		i->_edges.erase(find(i->_edges.begin(), i->_edges.end(), e1));
	    if (InnerNode *i = dynamic_cast<InnerNode*>(other2))
		i->_edges.erase(find(i->_edges.begin(), i->_edges.end(), e2));

	    // connecting other1 and other2 with a new edge
	    Edge *e = new Edge(other2, e1->length() + e2->length());
	    e->set_t1(other1);
	}
}


// FIXME: this only deletes ONE node, not them all!!!  I really should
// collect all the nodes and edges and delete them, but on the other
// hand, I never actually have to delete a tree until I complete the
// program, so there isn't really much point in doing a lot to free
// the tree...fix it if it becomes a problem later on!
Tree::~Tree() 
{ 
    delete _node; 
}
