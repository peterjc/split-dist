// Copyright (C) 2003 by BiRC -- Bioinformatics Research Center
//                               University of Aarhus, Denmark
//                               Contact: Thomas Mailund <mailund@birc.dk>

#include "config.hh"
#include "options.hh"

#if HAVE_LIBPOPT
#  include <popt.h>
#endif

#include <iostream>
using namespace std;

#include "tree.hh"
#include "parser.hh"
#include "label-map-visitor.hh"
#include "set-builder.hh"
#include "set-matcher.hh"

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
	"print-tree",
	'p',
	POPT_ARG_NONE,
	&options::print_tree,
	0,
	"Print tree with supported edges annotated.",
	0
    },
    {
	"output-file",
	'o',
	POPT_ARG_STRING,
	&options::output_fname,
	0,
	"Name of file to print output tree to.",
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
    const char *t1_fname;
    const char *t2_fname;

#if HAVE_LIBPOPT

    poptContext ctxt = poptGetContext(0, argc, argv, main_options, 0);
    poptSetOtherOptionHelp(ctxt, "tree1 tree2");

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
	       << "\n"
	       << "For submitting bug-reports, please use "
	       << '<' << PACKAGE_BUGREPORT << ">\n\n";

	  return 0;
      }

  t1_fname = poptGetArg(ctxt);
  t2_fname = poptGetArg(ctxt);
  if (t1_fname == 0 || t2_fname == 0)
      {
	  cerr << "Wrong number of arguments\n"
	       << "usage: qdist [options] tree1 tree2"
	       << endl;
	  return 2;
      }

  const char *extra = poptGetArg(ctxt);
  if (extra)
      {
	  cerr << "WARNING: Ignored options after tree files." << endl;
      }

#else  // ! HAVE_LIBPOPT
    if (argc != 3)
	{
	    cerr << "wrong number of arguments!\n"
		 << "usage: " << argv[0] << " first-tree second-tree"
		 << endl;
	    exit(1);
	}

    t1_fname = argv[1];
    t2_fname = argv[2];
#endif

    FILE *t1_fp = fopen(t1_fname, "r");
    if (!t1_fp)
	{
	    cerr << "could not open `" << t1_fname << "' : ";
	    perror(0);
	    exit(2);
	}
    FILE *t2_fp = fopen(t2_fname, "r");
    if (!t2_fp)
	{
	    cerr << "could not open `" << t2_fname << "' : ";
	    perror(0);
	    exit(2);
	}

    Tree *t1 = parse_file(t1_fp); fclose(t1_fp);
    if (!t1)
	{
	    cerr << "error parsing file " << t1_fname << endl;
	    exit(2);
	}
    Tree *t2 = parse_file(t2_fp); fclose(t2_fp);
    if (!t2)
	{
	    cerr << "error parsing file " << t2_fname << endl;
	    exit(2);
	}


    LabelMapVisitor lm;
    SetBuilder sb(lm);
    SetMatcher sm(lm,sb);

    t1->dfs_traverse(lm);
    t1->dfs_traverse(sb);
    t2->dfs_traverse(sm);

    cout << sm.match_count() << " out of " << sm.edge_count()
	 << " edges were supported\n";

    if (options::output_fname)
	{
	    FILE *f = freopen(options::output_fname, "w", stdout);
	    if (f == NULL)
		{
		    std::cerr << "Couldn't open output file `"
			      << options::output_fname << "'\n";
		    return 2;
		}
	    cout << *t1 << endl;
	}
    else if (options::print_tree)
	{
	    cout << *t1 << endl;
	}

    return 0;
}
