// Copyright (C) 2003, 2004 by BiRC -- Bioinformatics Research Center
//                             University of Aarhus, Denmark
//                             Contact: Thomas Mailund <mailund@birc.dk>

#include "split-set-count.hh"


namespace {
    typedef std::map<SplitSet,int> ss_count_map_t;
    ss_count_map_t _count_map;
}

void
split_set_count::inc_split_set_count(const SplitSet &ss)
{
    _count_map[ss]++;
}

split_set_count::ss_count_iterator_t
split_set_count::begin()
{
    return _count_map.begin();
}

split_set_count::ss_count_iterator_t
split_set_count::end()
{
    return _count_map.end();
}
