// Copyright (C) 2003, 2004 by BiRC -- Bioinformatics Research Center
//                             University of Aarhus, Denmark
//                             Contact: Thomas Mailund <mailund@birc.dk>

#include "config.hh"
#include "options.hh"

#if HAVE_LIBPOPT
#  include <popt.h>
#endif

#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

#include "tree.hh"
#include "parser.hh"
#include "label-map-visitor.hh"
#include "set-builder.hh"
#include "set-matcher.hh"
#include "result-counter.hh"
#include "split-set-count.hh"


#if HAVE_LIBPOPT
static struct poptOption info_options[] = {
    {
	"version",
	'\0',
	POPT_ARG_NONE,
	&options::version,
	0,
	"Print version of qdist.",
	0
    },
    {
	"contact",
	'\0',
	POPT_ARG_NONE,
	&options::contact,
	0,
	"Print contact information.",
	0
    },
    { 0 } // sentinel
};


static struct poptOption main_options[] = {
    {
	"verbose",
	'v',
	POPT_ARG_NONE,
	&options::verbose,
	0,
	"Toggle verbose output.",
	0
    },
    {
	"print-trees",
	'p',
	POPT_ARG_NONE,
	&options::print_trees,
	0,
	"Print trees with supported edges annotated.",
	0
    },
    {
	"print-all-splits",
	'\0',
	POPT_ARG_NONE,
	&options::print_all_splits,
	0,
	"Print all splits observed in the input trees (with their count).",
	0
    },
    {
	"print-shared-splits",
	'p',
	POPT_ARG_NONE,
	&options::print_shared_splits,
	0,
	"Print splits shared by all trees.",
	0
    },
    {
	0,
	'\0',
	POPT_ARG_INCLUDE_TABLE,
	&info_options,
	0,
	"Information options",
	0
    },

    POPT_AUTOHELP
    { 0 } // sentinel
};
#endif




int
main(int argc, const char *argv[])
{
    const char *fname;
    vector<const char*> tree_files;
    vector<Tree*> trees;

#if HAVE_LIBPOPT

    poptContext ctxt = poptGetContext(0, argc, argv, main_options, 0);
    poptSetOtherOptionHelp(ctxt, "trees");

  int opt = poptGetNextOpt(ctxt);
  if (opt < -1)
      {
	  std::cerr << poptBadOption(ctxt, POPT_BADOPTION_NOALIAS)
		    << ':' << poptStrerror(opt) << std::endl;
	  return 2;
      }

  if (options::version)
      {
	  cout << PACKAGE_STRING << endl;
	  return 0;
      }

  if (options::contact)
      {
	  cout << PACKAGE_STRING << "\n\n";
	  cout << "For questions or comments on sdist, contact:\n"
	       << "\tThomas Mailund <thomas@mailund.dk>, <mailund@birc.dk>\n"
	       << "\n";
	  return 0;
      }

  while ( (fname = poptGetArg(ctxt)) )
      {

#else  // ! HAVE_LIBPOPT

  for (i = 1; i < argc; ++i)
      {
	  fname = argv[i];
#endif

	  tree_files.push_back(fname);
	  FILE *fp = fopen(fname, "r");
	  if (!fp)
	      {
		  cerr << "could not open `" << fname << "' : ";
		  perror(0);
		  exit(2);
	      }
	  Tree *t = parse_file(fp); fclose(fp);
	  trees.push_back(t);
      }

    Tree::no_trees = trees.size();
    if (trees.size() < 2)
	{
	    cerr << "at least two trees expected!\n";
	    exit(2);
	}

    LabelMapVisitor lm;
    Tree *first = trees[0];
    try
	{
	    first->dfs_traverse(lm);
	}
    catch (LabelMap::AlreadyPushedEx ex)
	{
	    cerr << "The label '" << ex.label << "' appeard twice.\n";
	    exit(2);
	}
    

    cout << "Distance matrix:\n";
    for (unsigned int i = 0; i < trees.size(); ++i)
	{
	    Tree *main_tree = trees[i];
	    SetBuilder sb(lm);
	    Leaf *r1 = main_tree->find_leaf(lm.root_label());

	    cout << setw(10) << string(tree_files[i]).substr(0,10) << " : ";

	    try
		{
		    r1->dfs_traverse(sb);
		}
	    catch (LabelMap::UnkownLabelEx ex)
		{
		    cerr << "The label '" << ex.label
			 << "' was not in the " << tree_files[i] << " tree.\n";
		    return 2;
		}

	    for (unsigned int j = 0; j < trees.size(); ++j)
		{
		    if (i == j) { cout << " - " << ' '; continue; }

		    Tree *t = trees[j];
		    SetMatcher sm(lm,sb);
		    ResultCounter rs;
		    ClearSupported cs;

		    try
			{
			    Leaf *r2 = t->find_leaf(lm.root_label());

			    r2->dfs_traverse(sm);
			    r1->dfs_traverse(rs);
			    r1->dfs_traverse(cs);
			}
		    catch (LabelMap::UnkownLabelEx ex)
			{
			    cerr << "The label '" << ex.label
				 << "' was not in the "
				 << tree_files[j] << " tree.\n";
			    return 2;
			}

		    cout << setw(3) << rs.edge_count - rs.sup_count << ' ';
		}

	    cout << endl;

	}
    cout << endl;

    if (options::verbose)
	{
	    int no_splits = 0, no_splits_shared = 0;
	    split_set_count::ss_count_iterator_t i;
	    for (i = split_set_count::begin();
		 i != split_set_count::end(); ++i)
		if (!i->first.is_trivial())
		    {
			++no_splits;
			if (i->second == Tree::number_of_trees())
			    ++no_splits_shared;
		    }

	    cout << no_splits_shared << " out of "
		 << no_splits
		 << " non-trivial splits shared by all trees.\n\n";
	}
    

    if (options::print_shared_splits)
	{
	    cout << "Shared splits:\n";

	    split_set_count::ss_count_iterator_t i;
	    for (i = split_set_count::begin();
		 i != split_set_count::end(); ++i)
		if (!i->first.is_trivial()
		    and i->second == Tree::number_of_trees())
		    cout << i->first << endl;

	    cout << endl;
	}

    if (options::print_all_splits)
	{
	    cout << "All splits:\n";

	    split_set_count::ss_count_iterator_t i;
	    for (i = split_set_count::begin();
		 i != split_set_count::end(); ++i)
		if (!i->first.is_trivial())
		    cout << i->first << " : " << i->second << endl;

	    cout << endl;
	}

    if (options::print_trees)
	{
	    for (unsigned int i = 0; i < trees.size(); ++i)
		cout << tree_files[i] << " : " << *trees[i] << endl;
	}


    vector<Tree*>::const_iterator i;
    for (i = trees.begin(); i != trees.end(); ++i)
	delete *i;

    return 0;
}
