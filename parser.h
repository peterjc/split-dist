#ifndef BISON_Y_TAB_H
# define BISON_Y_TAB_H

#ifndef YYSTYPE
typedef union {
    Tree              *tree;
    Leaf              *leaf;
    std::list<Edge*>  *edge_list;
    
    char  *str;
    float  f;
} yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif
# define	FLOAT	257
# define	NAME	258


extern YYSTYPE yylval;

#endif /* not BISON_Y_TAB_H */
