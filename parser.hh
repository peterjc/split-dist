// Copyright (C) 2003 by BiRC -- Bioinformatics Research Center
//                               University of Aarhus, Denmark
//                               Contact: Thomas Mailund <mailund@birc.dk>

#ifndef PARSER_HH
#define PARSER_HH

#include "tree.hh"

Tree *parse_string (const char *str);
Tree *parse_file   (FILE       *fp);


#endif // PARSER_HH
