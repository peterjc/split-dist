// Copyright (C) 2003, 2004 by BiRC -- Bioinformatics Research Center
//                             University of Aarhus, Denmark
//                             Contact: Thomas Mailund <mailund@birc.dk>

#ifndef SPLITSET_HH
#define SPLITSET_HH

#include "bitset.hh"
#include "label-map.hh"

class SplitSet : public BitSet {
    const LabelMap &_lm;
    unsigned int _ones, _zeros;

public:
    SplitSet(const LabelMap &lm, const BitSet &bs);

    unsigned int ones()  const { return _ones; }
    unsigned int zeros() const { return _zeros; }

    unsigned int smallest_set() const { return std::min(_ones,_zeros); }

    // is it a trivial split, i.e., does it split in a singleton and
    // the rest?
    bool is_trivial() const { return _ones == 1 or _zeros == 1; }

    void print(std::ostream &) const;
};

inline std::ostream &
operator << (std::ostream &os, const SplitSet &ss)
{ ss.print(os); return os; }

#endif // SPLITSET_HH
