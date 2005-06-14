// Copyright (C) 2003, 2004 by BiRC -- Bioinformatics Research Center
//                             University of Aarhus, Denmark
//                             Contact: Thomas Mailund <mailund@birc.dk>

#ifndef TREE_HH
#define TREE_HH

#include <string>
#include <list>
#include <iostream>

class Tree;

// exception
struct no_node_with_high_degree {};

class Node {
    friend class Tree;

public:
    Node();
    virtual ~Node();
    virtual void connect(class Edge *e) = 0;
    virtual void dfs(class Visitor &v, const class Edge *from) = 0;
            void dfs_traverse(class Visitor &v);

    class Leaf *find_leaf(std::string name);
    virtual void print(std::ostream &os, const class Edge *from = 0) const = 0;
};

class Edge {
    friend class Tree;
    friend class InnerNode;

    Node *_t1;
    Node *_t2;
    double _length;

    int   _supported_count;

public:
    Edge(Node *t2, double length);
    void set_t1(Node *t1) { _t1 = t1; _t1->connect(this); }

    Node *t1() const { return _t1; }
    Node *t2() const { return _t2; }
    double length() const { return _length; }

    void tag_supported()         { ++_supported_count; }
    int  supported_count() const { return _supported_count; }

    void dfs(class Visitor &, const Node *from);
    void print(std::ostream &os, const class Node *from = 0) const;
};


class Leaf : public Node {
    friend class Tree;
    const std::string _name;
    Edge *_e;
public:
    Leaf(const char *name);
    std::string name() const { return _name; }
    virtual void connect(Edge *e);

    virtual void dfs(class Visitor &, const Edge *from);
    virtual void print(std::ostream &os, const class Edge *from = 0) const;
};

class InnerNode : public Node {
    friend class Tree;
    std::list<Edge*> _edges;
public:
    InnerNode(std::list<Edge*> *edges);
    virtual void connect(Edge *e);

    size_t no_edges() const { return _edges.size(); }

    virtual void dfs(class Visitor &, const Edge *from);
    virtual void print(std::ostream &os, const class Edge *from = 0) const;
};



class Tree {
    friend int main(int,const char**);
    static int no_trees;

    Node *_node;

public:
    Tree(Node *node);
    ~Tree();

    static int number_of_trees() { return no_trees; }

    Leaf *find_leaf(std::string name)   { return _node->find_leaf(name); }
    void dfs_traverse(class Visitor &v) { _node->dfs_traverse(v); }

    void print(std::ostream &os) const  { _node->print(os); }
};



inline std::ostream &operator<<(std::ostream &os, const Node &t)
{ t.print(os); return os; }
inline std::ostream &operator<<(std::ostream &os, const Edge &e)
{ e.print(os); return os; }


inline std::ostream &operator<<(std::ostream &os, const Tree &t)
{ t.print(os); return os; }



#endif // TREE_HH
