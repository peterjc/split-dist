// Copyright (C) 2003, 2004 by BiRC -- Bioinformatics Research Center
//                             University of Aarhus, Denmark
//                             Contact: Thomas Mailund <mailund@birc.dk>

#include "split-set.hh"

SplitSet::SplitSet(const LabelMap &lm, const BitSet &bs)
    : BitSet(bs), _lm(lm), _ones(0), _zeros(0)
{
    for (unsigned int i = 0; i < size(); ++i)
	if (at(i)) ++_ones;
	else       ++_zeros;
}

void
SplitSet::print(std::ostream &os) const
{
    for (unsigned int i = 0; i < size(); ++i)
	if (at(i)) os << _lm.name(i) << ' ';
    os << "| ";
    for (unsigned int i = 0; i < size(); ++i)
	if (!at(i)) os << _lm.name(i) << ' ';
}
