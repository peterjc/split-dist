// Copyright (C) 2003, 2004 by BiRC -- Bioinformatics Research Center
//                             University of Aarhus, Denmark
//                             Contact: Thomas Mailund <mailund@birc.dk>

#ifndef SPLIT_SET_COUNT_HH
#define SPLIT_SET_COUNT_HH

#include "split-set.hh"
#include <map>

namespace split_set_count {
    void inc_split_set_count(const SplitSet &ss);

    typedef std::map<SplitSet,int>::const_iterator ss_count_iterator_t;
    ss_count_iterator_t begin();
    ss_count_iterator_t end();
    
};

#endif // SPLITSET_HH
