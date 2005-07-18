// Copyright (C) 2003, 2004 by BiRC -- Bioinformatics Research Center
//                             University of Aarhus, Denmark
//                             Contact: Thomas Mailund <mailund@birc.dk>

#include "set-matcher.hh"

void
SetMatcher::handle_edge(BitSet *bs, Edge *e)
{
    Edge *split_edge = _map.lookup(bs);

    ++_edge_count;
    _edge_weight += e->length();
    if (split_edge)
	{
	    split_edge->tag_supported();
	    ++_sup_count;
	    _sup_weight += e->length();
	}

    
    // keep track of statistics for non-matching splits
    if (options::print_split_statistics and !split_edge)
	{
	    static LabelMap dummy;
	    SplitSet ss(dummy,*bs);
	    if (!ss.is_trivial())
		{
		    ++_non_matching_count;
		    unsigned int size = ss.smallest_set();
		    _max_small_set = std::max(size,_max_small_set);
		    _min_small_set = std::min(size,_min_small_set);
		    _small_set_sum += size;
		}
	}
}

