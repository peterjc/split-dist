// Copyright (C) 2003, 2004 by BiRC -- Bioinformatics Research Center
//                             University of Aarhus, Denmark
//                             Contact: Thomas Mailund <mailund@birc.dk>

#include "split-set.hh"

SplitSet::SplitSet(const LabelMap &lm, const BitSet &bs)
    : BitSet(bs), _lm(lm), _trivial(false)
{
    int ones, zeros;
    ones = zeros = 0;
    for (unsigned int i = 0; i < size(); ++i)
	if (at(i)) ++ones;
	else       ++zeros;
    if (ones == 1 or zeros == 1) _trivial = true;
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
