// Copyright (C) 2003, 2004 by BiRC -- Bioinformatics Research Center
//                             University of Aarhus, Denmark
//                             Contact: Thomas Mailund <mailund@birc.dk>

#ifndef OPTIONS_HH
#define OPTIONS_HH

namespace options {
    extern int version;
    extern int contact;

    extern int silent;
    extern int verbose;

    extern int print_norm_dist;
    extern int print_rf_dist;
    extern int print_similarity;

    extern int print_weighted_dist;
    extern int print_weighted_sim;
    extern int print_norm_weighted_dist;
    extern int print_norm_weighted_sim;

    extern int print_split_statistics;

    extern int print_all_splits;
    extern int print_shared_splits;

    extern int print_trees;
};


#endif
