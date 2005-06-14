/* -*- C++ -*- */
/* Copyright (C) 2003 by BiRC -- Bioinformatics Research Center
                                 University of Aarhus, Denmark
                                 Contact: Thomas Mailund <mailund@birc.dk>
*/

%{

#define YY_NO_UNPUT

#include <iostream>
using namespace std;

#include "tree.hh"
#include <list> 
#include "parser.h" 

static int current_line = 1;


%}

D                               [0-9]
L                               [a-zA-Z_/-]
FLOAT                           -?{D}+|-?[0-9]*\.[0-9]+|(-?[0-9]*\.[0-9]+|-?{D}+)?[eE][+-]?{D}+

%%

"("				return '(';
")"				return ')';
","				// ignore commas
";"				// ignore semicolons

":"				return ':';
{FLOAT}				yylval.f = atof(yytext); return FLOAT;

[a-zA-Z][^:(),; \t\n]*		yylval.str = yytext; return NAME;
'[^']*'				yytext[strlen(yytext)-1] = '\0'; yylval.str = yytext+1; return NAME;


\n				current_line++;
[\t ]				// ignore whitespace



.               { 
  cerr << "illegal character in input `"
       << yytext[0] << "' at line "
       << current_line << '\n';
  exit(1);
}

%%

int
yywrap (void)
{
    return 1;
}

int
lex_get_line (void)
{
    return current_line;
}

void
lex_reset_line_number (void)
{
    current_line = 1;
}
