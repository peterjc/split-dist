// Copyright (C) 2003, 2004 by BiRC -- Bioinformatics Research Center
//                             University of Aarhus, Denmark
//                             Contact: Thomas Mailund <mailund@birc.dk>

#ifndef SET_MATCHER_HH
#define SET_MATCHER_HH

#include "set-traverser.hh"
#include "set-builder.hh"
#include "split-set.hh"
#include "options.hh"

#include <limits>

class SetMatcher : public SetTraverser {
    SetBuilder   &_map;
    unsigned int _edge_count, _sup_count, _non_matching_count;
    unsigned int _max_small_set, _min_small_set, _small_set_sum;

public:
    SetMatcher(LabelMap &lm, SetBuilder &sb)
	: SetTraverser(lm,true), _map(sb),
	  _edge_count(0), _sup_count(0), _non_matching_count(0),
	  _max_small_set(0),
	  _min_small_set(std::numeric_limits<unsigned int>::max()),
	  _small_set_sum(0)
    {}
    virtual ~SetMatcher() {}

    unsigned int edge_count() const { return _edge_count; }
    unsigned int sup_count()  const { return _sup_count; }

    unsigned int max_small_set() const { return _max_small_set; }
    unsigned int min_small_set() const { return _min_small_set; }
    double       avg_small_set() const
    { if (_non_matching_count == 0) return 0.0;
      else return static_cast<double>(_small_set_sum)/_non_matching_count; }

    virtual void handle_edge(BitSet *bs, Edge *) {
	Edge *split_edge = _map.lookup(bs);

	++_edge_count;
	if (split_edge)
	    {
		split_edge->tag_supported();
		++_sup_count;
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
};

#endif // SET_MATCHER_HH
