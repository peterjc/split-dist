/* -*- C++ -*- */
// Copyright (C) 2003 by BiRC -- Bioinformatics Research Center
//                               University of Aarhus, Denmark
//                               Contact: Thomas Mailund <mailund@birc.dk>

%{

#include <iostream>
#include <list>
using namespace std;

int yylex(void);
void yyerror(char *msg);

int lex_get_line (void);
void lex_reset_line_number (void); 

#include "tree.hh"
extern Node *yy_tree; 		// forward decl.
 
%}

%union {
    Node              *tree;
    Leaf              *leaf;
    std::list<Edge*>  *edge_list;
    
    char  *str;
    double  f;
};

%type<tree> tree
%type<edge_list> edge_list
%type<leaf> leaf

%token FLOAT NAME
%type<f> FLOAT
%type<str> NAME


%%

top: tree 			{ yy_tree = $1; }
  ;

tree:
    '(' edge_list ')' 		{ $$ = new InnerNode($2); delete $2; }
 //| '(' edge_list ')' ':' FLOAT { $$ = new InnerNode($2); delete $2; }
 //| '(' edge_list ')' FLOAT ':' FLOAT { $$ = new InnerNode($2); delete $2; }
  | leaf 			{ $$ = $1; }
  ;

edge_list:
    tree edge_list 		{ $$ = $2; $$->push_back(new Edge($1,0.0)); }
  | tree ':' FLOAT edge_list 	{ $$ = $4; $$->push_back(new Edge($1,$3)); }
  | tree FLOAT ':' FLOAT edge_list { $$ = $5; $$->push_back(new Edge($1,$4)); }
  | /* empty */			{ $$ = new list<Edge*>(); }
  ;

leaf:
  NAME 				{ $$ = new Leaf($1); }
  ;

%%

void
yyerror (char *errmsg)
{
    extern char *yytext;
    cerr << "parse error in line " << lex_get_line()
	 << ": error before `" << yytext << "'\n";
}


// stuff from lexer.cc
typedef struct yy_buffer_state *YY_BUFFER_STATE;
YY_BUFFER_STATE yy_scan_string(const char *str);
void yy_delete_buffer(YY_BUFFER_STATE b);
YY_BUFFER_STATE yy_create_buffer(FILE *file, int size);
void yy_switch_to_buffer(YY_BUFFER_STATE new_buffer);


Node *yy_tree; 

Tree *
parse_string(const char *str)
{
    int res;

    lex_reset_line_number();
    YY_BUFFER_STATE buf = yy_scan_string(str);
    res = yyparse();
    yy_delete_buffer(buf);

    return (res == 0) ? new Tree(yy_tree) : 0;
}

/* Size of default input buffer. -- got this guy from lexer.cc */
#define YY_BUF_SIZE 16384

Tree *
parse_file(FILE *fp)
{
    int res;

    lex_reset_line_number();
    YY_BUFFER_STATE parser_buf = yy_create_buffer(fp, YY_BUF_SIZE);
    yy_switch_to_buffer(parser_buf);
    res = yyparse();
    yy_delete_buffer(parser_buf);

    return (res == 0) ? new Tree(yy_tree) : 0;
}

