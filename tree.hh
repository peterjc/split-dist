// Copyright (C) 2003, 2004 by BiRC -- Bioinformatics Research Center
//                             University of Aarhus, Denmark
//                             Contact: Thomas Mailund <mailund@birc.dk>

#ifndef TREE_HH
#define TREE_HH

#include <string>
#include <list>
#include <iostream>

class Tree {
public:
    Tree();
    virtual ~Tree();
    virtual void connect(class Edge *e) = 0;
    virtual void dfs(class Visitor &v, const class Edge *from) = 0;
            void dfs_traverse(class Visitor &v);

    class Leaf *find_leaf(std::string name);

    virtual void print(std::ostream &os, const class Edge *from = 0) const = 0;

private:
    friend int main(int,const char**);
    static int no_trees;
public:
    static int number_of_trees() { return no_trees; }
};

class Edge {
    friend class InnerNode;

    Tree *_t1;
    Tree *_t2;
    float _length;

    int   _supported_count;

public:
    Edge(Tree *t2, float length);
    void set_t1(Tree *t1) { _t1 = t1; _t1->connect(this); }

    Tree *t1() const { return _t1; }
    Tree *t2() const { return _t2; }

    void tag_supported()         { ++_supported_count; }
    int  supported_count() const { return _supported_count; }

    void dfs(class Visitor &, const Tree *from);
    void print(std::ostream &os, const class Tree *from = 0) const;
};


class Leaf : public Tree {
    const std::string _name;
    Edge *_e;
public:
    Leaf(const char *name);
    std::string name() const { return _name; }
    virtual void connect(Edge *e);

    virtual void dfs(class Visitor &, const Edge *from);
    virtual void print(std::ostream &os, const class Edge *from = 0) const;
};

class InnerNode : public Tree {
    std::list<Edge*> _edges;
public:
    InnerNode(std::list<Edge*> *edges);
    virtual void connect(Edge *e);

    virtual void dfs(class Visitor &, const Edge *from);
    virtual void print(std::ostream &os, const class Edge *from = 0) const;
};



inline std::ostream &operator<<(std::ostream &os, const Tree &t)
{ t.print(os); return os; }
inline std::ostream &operator<<(std::ostream &os, const Edge &e)
{ e.print(os); return os; }




#endif // TREE_HH
