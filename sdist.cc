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
	"silent",
	's',
	POPT_ARG_NONE,
	&options::silent,
	0,
	"Produce minimal output.",
	0
    },

    {
	"print-norm",
	'\0',
	POPT_ARG_NONE,
	&options::print_norm_dist,
	0,
	"Print normalized distance (distance/total-splits).",
	0
    },
    {
	"rf",
	'\0',
	POPT_ARG_NONE,
	&options::print_rf_dist,
	0,
	"Print Robinson-Foulds distance ((false-positives+false-negatives)/2).",
	0
    },
    {
	"print-sim",
	'\0',
	POPT_ARG_NONE,
	&options::print_similarity,
	0,
	"Print similarity (shared-splits/total-splits).",
	0
    },

    {
	"print-weighted-distance",
	'w',
	POPT_ARG_NONE,
	&options::print_weighted_dist,
	0,
	"Print distance where edges are weighted by their length.",
	0
    },
    {
	"print-weighted-similarity",
	'\0',
	POPT_ARG_NONE,
	&options::print_weighted_sim,
	0,
	"Print similarity where edges are weighted by their length.",
	0
    },
    {
	"print-norm-weighted-distance",
	'\0',
	POPT_ARG_NONE,
	&options::print_norm_weighted_dist,
	0,
	"Print normalized distance where edges are weighted by their length.",
	0
    },
    {
	"print-norm-weighted-similarity",
	'\0',
	POPT_ARG_NONE,
	&options::print_norm_weighted_sim,
	0,
	"Print normalized similarity where edges are weighted by their length.",
	0
    },

    {
	"print-split-statistics",
	'\0',
	POPT_ARG_NONE,
	&options::print_split_statistics,
	0,
	"Print information about the size of the non-matching splits (the size"
	" of the smallest set in the split).",
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
	'\0',
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


// FIXME: refactor these, they share a *lot* of code
static void
print_dist_matrix(size_t label_width,
		  vector< vector<unsigned int> > &split_dist_matrix,
		  const vector<const char*> &tree_files)
{
    size_t no_trees = split_dist_matrix.size();
    cout << "Distance Matrix:\n"
	 << "----------------\n";
    for (unsigned int i = 0; i < no_trees; ++i)
	{
	    cout << setw(label_width)
		 << string(tree_files[i]).substr(0,label_width) << " : ";
	    for (unsigned int j = 0; j < no_trees; ++j)
		{
		    if (i == j)
			cout << "  -" << ' ';
		    else
			// FIXME: width shouldn't be fixed at 3
			cout << setw(3) << split_dist_matrix[i][j] << ' ';
		}
	    cout << endl;
	}
    cout << endl;
}

static void
print_norm_dist_matrix(size_t label_width,
		       vector< vector<double> > &norm_dist_matrix,
		       const vector<const char*> &tree_files)
{
    size_t no_trees = norm_dist_matrix.size();
    cout << "Normalized Distance Matrix:\n"
	 << "---------------------------\n";
    for (unsigned int i = 0; i < no_trees; ++i)
	{
	    cout << setw(label_width)
		 << string(tree_files[i]).substr(0,label_width) << " : ";
	    for (unsigned int j = 0; j < no_trees; ++j)
		{
		    if (i == j)
			cout << "       -" << ' ';
		    else
			// FIXME: width shouldn't be fixed at 6
			cout << setw(6) << norm_dist_matrix[i][j] << ' ';
		}
	    cout << endl;
	}
    cout << endl;
}

static void
print_rf_matrix(size_t label_width,
		vector< vector<unsigned int> > &split_dist_matrix,
		const vector<const char*> &tree_files)
{
    size_t no_trees = split_dist_matrix.size();
    cout << "Robinson-Foulds Matrix:\n"
	 << "-----------------------\n";
    for (unsigned int i = 0; i < no_trees; ++i)
	{
	    cout << setw(label_width)
		 << string(tree_files[i]).substr(0,label_width) << " : ";
	    for (unsigned int j = 0; j < no_trees; ++j)
		{
		    if (i == j)
			cout << "  -" << ' ';
		    else
			// FIXME: width shouldn't be fixed at 3
			cout << setw(3)
			     << double(split_dist_matrix[i][j]
				       + split_dist_matrix[j][i])/2
			     << ' ';
		}
	    cout << endl;
	}
    cout << endl;
}


static void
print_similarity(size_t label_width,
		 vector< vector<double> > &sim_matrix,
		 const vector<const char*> &tree_files)
{
    size_t no_trees = sim_matrix.size();
    cout << "Similarity Matrix:\n"
	 << "------------------\n";
    for (unsigned int i = 0; i < no_trees; ++i)
	{
	    cout << setw(label_width)
		 << string(tree_files[i]).substr(0,label_width) << " : ";
	    for (unsigned int j = 0; j < no_trees; ++j)
		{
		    if (i == j)
			cout << "       -" << ' ';
		    else
			// FIXME: width shouldn't be fixed at 6
			cout << setw(6) << sim_matrix[i][j] << ' ';
		}
	    cout << endl;
	}
    cout << endl;
}

static void
print_weighted_dist_matrix(size_t label_width,
			   vector< vector<double> > &weighted_dist_matrix,
			   const vector<const char*> &tree_files)
{
    size_t no_trees = weighted_dist_matrix.size();
    cout << "Weighted Distance Matrix:\n"
	 << "-------------------------\n";
    for (unsigned int i = 0; i < no_trees; ++i)
	{
	    cout << setw(label_width)
		 << string(tree_files[i]).substr(0,label_width) << " : ";
	    for (unsigned int j = 0; j < no_trees; ++j)
		{
		    if (i == j)
			cout << "       -" << ' ';
		    else
			// FIXME: width shouldn't be fixed at 6
			cout << setw(6) << weighted_dist_matrix[i][j] << ' ';
		}
	    cout << endl;
	}
    cout << endl;
}

static void
print_weighted_similarity(size_t label_width,
			  vector< vector<double> > &sim_matrix,
			  const vector<const char*> &tree_files)
{
    size_t no_trees = sim_matrix.size();
    cout << "Weighted Similarity Matrix:\n"
	 << "---------------------------\n";
    for (unsigned int i = 0; i < no_trees; ++i)
	{
	    cout << setw(label_width)
		 << string(tree_files[i]).substr(0,label_width) << " : ";
	    for (unsigned int j = 0; j < no_trees; ++j)
		{
		    if (i == j)
			cout << "       -" << ' ';
		    else
			// FIXME: width shouldn't be fixed at 6
			cout << setw(6) << sim_matrix[i][j] << ' ';
		}
	    cout << endl;
	}
    cout << endl;
}

static void
print_norm_weighted_dist_matrix(size_t label_width,
			   vector< vector<double> > &weighted_dist_matrix,
			   const vector<const char*> &tree_files)
{
    size_t no_trees = weighted_dist_matrix.size();
    cout << "Normalized weighted Distance Matrix:\n"
	 << "------------------------------------\n";
    for (unsigned int i = 0; i < no_trees; ++i)
	{
	    cout << setw(label_width)
		 << string(tree_files[i]).substr(0,label_width) << " : ";
	    for (unsigned int j = 0; j < no_trees; ++j)
		{
		    if (i == j)
			cout << "       -" << ' ';
		    else
			// FIXME: width shouldn't be fixed at 6
			cout << setw(6) << weighted_dist_matrix[i][j] << ' ';
		}
	    cout << endl;
	}
    cout << endl;
}

static void
print_norm_weighted_similarity(size_t label_width,
			       vector< vector<double> > &sim_matrix,
			       const vector<const char*> &tree_files)
{
    size_t no_trees = sim_matrix.size();
    cout << "Normalized weighted Similarity Matrix:\n"
	 << "--------------------------------------\n";
    for (unsigned int i = 0; i < no_trees; ++i)
	{
	    cout << setw(label_width)
		 << string(tree_files[i]).substr(0,label_width) << " : ";
	    for (unsigned int j = 0; j < no_trees; ++j)
		{
		    if (i == j)
			cout << "       -" << ' ';
		    else
			// FIXME: width shouldn't be fixed at 6
			cout << setw(6) << sim_matrix[i][j] << ' ';
		}
	    cout << endl;
	}
    cout << endl;
}




static void
print_split_statistics(size_t label_width,
		       vector< vector<unsigned int> > &max_small_set,
		       vector< vector<unsigned int> > &min_small_set,
		       vector< vector<double> > &avg_small_set,
		       const vector<const char*> &tree_files)
{
    size_t no_trees = max_small_set.size();

    unsigned int min_val, max_val, sum;

    cout << "Split statistics:\n"
	 << "-----------------\n";
    if (!options::silent) cout << endl;

    cout << "Max size of smallest set:\n";
    min_val = numeric_limits<unsigned int>::max();
    max_val = 0;
    sum = 0;
    for (unsigned int i = 0; i < no_trees; ++i)
	{
	    if (!options::silent)
		cout << setw(label_width)
		     << string(tree_files[i]).substr(0,label_width) << " : ";
	    for (unsigned int j = 0; j < no_trees; ++j)
		{
		    if (i == j)
			{
			    if (!options::silent) cout << "  -" << ' ';
			}
		    else
			{
			    min_val = std::min(min_val,max_small_set[i][j]);
			    max_val = std::max(max_val,max_small_set[i][j]);
			    sum += max_small_set[i][j];

			    // FIXME: width shouldn't be fixed at 3
			    if (!options::silent)
				cout << setw(3) << max_small_set[i][j] << ' ';
			}
		}
	    if (!options::silent) cout << endl;
	}
    if (!options::silent) cout << endl;
    cout << "max in matrix: " << max_val << '\n'
	 << "min in matrix: " << min_val << '\n'
	 << "avg of matrix: "
	 << static_cast<double>(sum)/(no_trees*(no_trees-1))
	 << endl << endl;

    cout << "Min size of smallest set:\n";
    min_val = numeric_limits<unsigned int>::max();
    max_val = 0;
    sum = 0;
    for (unsigned int i = 0; i < no_trees; ++i)
	{
	    if (!options::silent)
		cout << setw(label_width)
		     << string(tree_files[i]).substr(0,label_width) << " : ";
	    for (unsigned int j = 0; j < no_trees; ++j)
		{
		    if (i == j)
			{
			    if (!options::silent) cout << "  -" << ' ';
			}
		    else
			{
			    min_val = std::min(min_val,min_small_set[i][j]);
			    max_val = std::max(max_val,min_small_set[i][j]);
			    sum += min_small_set[i][j];

			    // FIXME: width shouldn't be fixed at 3
			    if (!options::silent)
				cout << setw(3) << min_small_set[i][j] << ' ';
			}
		}
	    if (!options::silent) cout << endl;
	}
    if (!options::silent) cout << endl;
    cout << "max in matrix: " << max_val << '\n'
	 << "min in matrix: " << min_val << '\n'
	 << "avg of matrix: "
	 << static_cast<double>(sum)/(no_trees*(no_trees-1))
	 << endl << endl;

    double min_val_d, max_val_d, sum_d;
    cout << "Average size of smallest set:\n";
    min_val_d = numeric_limits<double>::max();
    max_val_d = 0.0;
    sum_d = 0.0;
    for (unsigned int i = 0; i < no_trees; ++i)
	{
	    if (!options::silent)
		cout << setw(label_width)
		     << string(tree_files[i]).substr(0,label_width) << " : ";
	    for (unsigned int j = 0; j < no_trees; ++j)
		{
		    if (i == j)
			{
			    if (!options::silent) cout << "     -" << ' ';
			}
		    else
			{
			    min_val_d = std::min(min_val_d,
						 avg_small_set[i][j]);
			    max_val_d = std::max(max_val_d,
						 avg_small_set[i][j]);
			    sum_d += avg_small_set[i][j];

			    // FIXME: format shouldn't be fixed
			    if (!options::silent)
				cout << setprecision(2) << fixed
				     << setw(6)
				     << avg_small_set[i][j] << ' ';
			}
		}
	    if (!options::silent) cout << endl;
	}
    if (!options::silent) cout << endl;
    cout << "max in matrix: " << max_val_d << '\n'
	 << "min in matrix: " << min_val_d << '\n'
	 << "avg of matrix: "
	 << sum_d/(no_trees*(no_trees-1))
	 << endl << endl;
}

static void
print_shared_splits()
{
    cout << "Shared Splits:\n"
	 << "--------------\n";
    split_set_count::ss_count_iterator_t i;
    for (i = split_set_count::begin();
	 i != split_set_count::end(); ++i)
	if (!i->first.is_trivial()
	    and i->second == Tree::number_of_trees())
	    cout << i->first << endl;
    cout << endl;
}

static void
print_all_splits()
{
    cout << "All Splits:\n"
	 << "-----------\n";
    split_set_count::ss_count_iterator_t i;
    for (i = split_set_count::begin();
	 i != split_set_count::end(); ++i)
	if (!i->first.is_trivial())
	    cout << i->first << " : " << i->second << endl;
    cout << endl;
}

int
main(int argc, const char *argv[])
{
    try {

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
		     << "\tThomas Mailund <mailund@birc.dk>\n"
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
		if (!t)
		    {
			cerr << "in file `" << fname << "'\n";
			exit(2);
		    }
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
	       cerr << "The label '" << ex.label << "' appeard twice in "
		    << tree_files[0] << '\n';
	       exit(2);
	   }
       
       // validating equal label sets
       for (unsigned int i = 1; i < trees.size(); ++i)
	   {
	       LabelMapVisitor lm2;
	       try
		   {
		       trees[i]->dfs_traverse(lm2);
		   }
	       catch (LabelMap::AlreadyPushedEx ex)
		   {
		       cerr << "The label '" 
			    << ex.label << "' appeard twice in "
			    << tree_files[i] << '\n';
		       exit(2);
		   }
	       
	       if (lm.size() != lm2.size())
		   {
		       cerr << "The label set of " << tree_files[0] << " and "
			    << tree_files[i] << " differs\n";
		       return 2;
		   }
	       
	       for (unsigned int n = 0; n < lm.size(); ++n)
		   {
		       try
			   {
			       lm2[lm.name(n)];
			   }
		       catch (LabelMap::UnkownLabelEx ex)
			   {
			       cerr << "The label '" << ex.label
				    << "' was not in the " << tree_files[i]
				    << " tree.\n";
			       return 2;
			   }
		   }
	   }


       vector< vector<unsigned int> >
	   split_dist_matrix(trees.size(), vector<unsigned int>(trees.size()));

       vector< vector<double> >
	   norm_dist_matrix(trees.size(), vector<double>(trees.size()));
       vector< vector<double> >
	   sim_matrix(trees.size(), vector<double>(trees.size()));

       vector< vector<double> >
	   weighted_dist_matrix(trees.size(), vector<double>(trees.size()));
       vector< vector<double> >
	   weighted_sim_matrix(trees.size(), vector<double>(trees.size()));
       vector< vector<double> >
	   norm_weighted_dist_matrix(trees.size(), vector<double>(trees.size()));
       vector< vector<double> >
	   norm_weighted_sim_matrix(trees.size(), vector<double>(trees.size()));

       
       vector< vector<unsigned int> >
	   max_small_set(trees.size(), vector<unsigned int>(trees.size()));
       vector< vector<unsigned int> >
	   min_small_set(trees.size(), vector<unsigned int>(trees.size()));
       vector< vector<double> >
	   avg_small_set(trees.size(), vector<double>(trees.size()));
       
       for (unsigned int i = 0; i < trees.size(); ++i)
	   {
	       Tree *main_tree = trees[i];
	       SetBuilder sb(lm);
	       Leaf *r1 = main_tree->find_leaf(lm.root_label());
	       
	       try
		   {
		       r1->dfs_traverse(sb);
		   }
	       catch (LabelMap::UnkownLabelEx ex)
		   {
		       cerr << "The label '" << ex.label
			    << "' was not in the " 
			    << tree_files[i] << " tree.\n";
		       return 2;
		   }
	       
	       for (unsigned int j = 0; j < trees.size(); ++j)
		   {
		       if (i == j)
			   {
			       split_dist_matrix[i][j] = 0;
			       continue;
			   }
		       
		       Tree *t = trees[j];
		       SetMatcher sm(lm,sb);
		       
		       try
			   {
			       Leaf *r2 = t->find_leaf(lm.root_label());
			       r2->dfs_traverse(sm);
			   }
		       catch (LabelMap::UnkownLabelEx ex)
			   {
			       cerr << "The label '" << ex.label
				    << "' was not in the "
				    << tree_files[j] << " tree.\n";
			       return 2;
			   }
		       
		       unsigned int dist = sm.edge_count() - sm.sup_count();
		       double wdist = sm.edge_weight() - sm.sup_weight();;
		       
		       // NB: ji here since we calculate the number of
		       // edges found in j not found in i, but we want to
		       // report i the other way around.
		       split_dist_matrix[j][i] = dist;

		       if (options::print_weighted_dist)
			   weighted_dist_matrix[j][i] = wdist;
		       if (options::print_weighted_sim)
			   weighted_sim_matrix[j][i] = sm.sup_weight();

		       if (options::print_norm_weighted_dist)
			   norm_weighted_dist_matrix[j][i] = 
			       wdist/sm.edge_weight();
		       if (options::print_norm_weighted_sim)
			   norm_weighted_sim_matrix[j][i] = 
			       sm.sup_weight()/sm.edge_weight();

		       if (options::print_norm_dist)
			   norm_dist_matrix[j][i] = double(dist)/sm.edge_count();
		       if (options::print_similarity)
			   sim_matrix[j][i] = double(sm.sup_count())/sm.edge_count();
		       
		       if (options::print_split_statistics)
			   {
			       max_small_set[j][i] = sm.max_small_set();
			       min_small_set[j][i] = sm.min_small_set();
			       avg_small_set[j][i] = sm.avg_small_set();
			   }
		   }
	   }
       
       int no_splits = 0, no_splits_shared = 0;
       if (options::verbose || options::silent)
	   {
	       split_set_count::ss_count_iterator_t i;
	       for (i = split_set_count::begin();
		    i != split_set_count::end(); ++i)
		   if (!i->first.is_trivial())
		       {
			   ++no_splits;
			   if (i->second == Tree::number_of_trees())
			       ++no_splits_shared;
		       }
	   }
       
       size_t label_width = 0;
       vector<const char*>::const_iterator itr;
       for (itr = tree_files.begin(); itr != tree_files.end(); ++itr)
	   label_width = max(label_width,strlen(*itr));
       
       if (!options::silent)
	   print_dist_matrix(label_width,split_dist_matrix, tree_files);
       
       if (options::print_norm_dist)
	   print_norm_dist_matrix(label_width,norm_dist_matrix, tree_files);
       
       if (options::print_rf_dist)
	   print_rf_matrix(label_width,split_dist_matrix, tree_files);
       
       if (options::print_similarity)
	   print_similarity(label_width,sim_matrix, tree_files);
       

       if (options::print_weighted_dist)
	   print_weighted_dist_matrix(label_width, 
				      weighted_dist_matrix,
				      tree_files);
       if (options::print_weighted_sim)
	   print_weighted_similarity(label_width, 
				     weighted_sim_matrix,
				     tree_files);

       if (options::print_norm_weighted_dist)
	   print_norm_weighted_dist_matrix(label_width, 
					   norm_weighted_dist_matrix,
					   tree_files);
       if (options::print_norm_weighted_sim)
	   print_norm_weighted_similarity(label_width, 
					  norm_weighted_sim_matrix,
					  tree_files);
       

       if (options::print_split_statistics)
	   print_split_statistics(label_width,
				  max_small_set,min_small_set,avg_small_set,
				  tree_files);
       
       if (options::print_shared_splits) print_shared_splits();
       if (options::print_all_splits)    print_all_splits();
       
       if (options::print_trees)
	   {
	       cout << "Annotated Nodes:\n"
		    << "----------------\n";
	       for (unsigned int i = 0; i < trees.size(); ++i)
		   cout << tree_files[i] << " : " << *trees[i] << endl;
	   }
       
       
       if (options::silent && trees.size() == 2)
	   cout << split_dist_matrix[0][1]
		<< " edges found in the first tree but not in the second.\n";
       if (options::verbose || (options::silent && trees.size() > 2))
	   cout << no_splits_shared << " out of "
		<< no_splits
		<< " non-trivial splits shared by all trees.\n";
       
       
       vector<Tree*>::const_iterator i;
       for (i = trees.begin(); i != trees.end(); ++i)
	   delete *i;
       
    } catch (no_node_with_high_degree&) {
	std::cerr << "At least one input tree contains no node with\n"
		  << "degree above 2 -- calculating the split distance\n"
		  << "is therefore meaningless.\n";
	return 2;
    }

    return 0;
}
