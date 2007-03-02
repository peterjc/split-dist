// Copyright (C) 2003 by BiRC -- Bioinformatics Research Center
//                               University of Aarhus, Denmark
//                               Contact: Thomas Mailund <mailund@birc.dk>

#include "label-map.hh"

using namespace std;

size_t
LabelMap::push(string label) throw(AlreadyPushedEx)
{
    if (_map.find(label) != _map.end()) throw AlreadyPushedEx();
    return _map[label] = _count++;
}

size_t
LabelMap::operator[](string label) const throw(UnkownLabelEx)
{
    map<string,size_t>::const_iterator i = _map.find(label);
    if (i == _map.end()) throw UnkownLabelEx();
    return i->second;
}