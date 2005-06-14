// Copyright (C) 2003, 2004 by BiRC -- Bioinformatics Research Center
//                             University of Aarhus, Denmark
//                             Contact: Thomas Mailund <mailund@birc.dk>

#include "tree.hh"
#include "parser.hh"

#include <cassert>
#include <iostream>
#include <sstream>
using namespace std;

bool
check_parse_string(const char *str)
{
    Tree *t = parse_string(str);
    bool res = t != 0;
    delete t;
    return res;
}

int
main(int argc, const char *argv[])
{
    cout << "\nThe parser test will print a number of parse errors.\n"
	 << "Do not worry about it, unless it exits a non-null value\n"
	 << "it isn't actually mistakes.\n\n";

    assert(check_parse_string("()"));
    assert(check_parse_string("();"));
    assert(check_parse_string("'A'"));


    assert(! check_parse_string(""));
    assert(check_parse_string("('A')"));

    try {
	check_parse_string("('A' 'B')");
	assert(false);
    } catch(no_node_with_high_degree&) {};
    try {
	check_parse_string("('A', 'B')");
	assert(false);
    } catch(no_node_with_high_degree&) {};

    assert(check_parse_string("('A':0.0)"));

    try {
	assert(check_parse_string("('A':0.0 'B':10.1)"));
	assert(false);
    } catch(no_node_with_high_degree&) {};
    try {
	assert(check_parse_string("('A':.98, 'B')"));
	assert(false);
    } catch(no_node_with_high_degree&) {};



    Tree *t = parse_string("('A':0.0, ('B':10.1, 'C':0.1), 'D')");

    ostringstream os1;
    os1 << *t << endl;

    assert(os1.str() == string("('D' 1 : 0, ('C' 1 : 0.1, 'B' 1 : 10.1) 1 : 0, 'A' 1 : 0)\n"));


    t = parse_string("(A:0.0, (B:10.1, C:0.1), D)");

    ostringstream os2;
    os2 << *t << endl;

    assert(os2.str() == string("('D' 1 : 0, ('C' 1 : 0.1, 'B' 1 : 10.1) 1 : 0, 'A' 1 : 0)\n"));

    t = parse_string("(A-_:0.0, (B:10.1, C:0.1), D)");

    ostringstream os3;
    os3 << *t << endl;

    assert(os3.str() == string("('D' 1 : 0, ('C' 1 : 0.1, 'B' 1 : 10.1) 1 : 0, 'A-_' 1 : 0)\n"));


    return 0;
}
