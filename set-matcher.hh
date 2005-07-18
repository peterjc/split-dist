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
    double _edge_weight, _sup_weight;
    unsigned int _max_small_set, _min_small_set, _small_set_sum;

public:
    SetMatcher(LabelMap &lm, SetBuilder &sb)
	: SetTraverser(lm,true), _map(sb),
	  _edge_count(0), _sup_count(0), _non_matching_count(0),
	  _edge_weight(0), _sup_weight(0),
	  _max_small_set(0),
	  _min_small_set(std::numeric_limits<unsigned int>::max()),
	  _small_set_sum(0)
    {}
    virtual ~SetMatcher() {}

    unsigned int edge_count()    const { return _edge_count; }
    unsigned int sup_count()     const { return _sup_count; }

    double edge_weight()         const { return _edge_weight; }
    double sup_weight()          const { return _sup_weight; }

    unsigned int max_small_set() const { return _max_small_set; }
    unsigned int min_small_set() const { return _min_small_set; }
    double       avg_small_set() const
    { if (_non_matching_count == 0) return 0.0;
      else return static_cast<double>(_small_set_sum)/_non_matching_count; }

    virtual void handle_edge(BitSet *bs, Edge *);
};

#endif // SET_MATCHER_HH
