// Copyright (C) 2003 by BiRC -- Bioinformatics Research Center
//                               University of Aarhus, Denmark
//                               Contact: Thomas Mailund <mailund@birc.dk>

#ifndef LABEL_MAP_HH
#define LABEL_MAP_HH

#include "tree.hh"

#include <map>
#include <string>

class LabelMap {
    size_t _count;
    std::map<std::string,size_t> _map;

public:
    LabelMap() : _count(0) {};

    class AlreadyPushedEx {};
    class UnkownLabelEx {};

    size_t push(std::string label) throw(AlreadyPushedEx);
    size_t operator[](std::string label) const throw(UnkownLabelEx);
    size_t size() const { return _count; }
};

#endif // LABEL_MAP_HH
