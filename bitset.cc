// Copyright (C) 2003, 2004 by BiRC -- Bioinformatics Research Center
//                             University of Aarhus, Denmark
//                             Contact: Thomas Mailund <mailund@birc.dk>

#include "bitset.hh"

#include <cassert>
#include <algorithm>
#include <functional>
using namespace std;

// -- CONSTRUCTION AND DESTRUCTION --------------------------------------
BitSet::BitSet(size_t size)
    : _size(size)
{
    _data = new size_t[array_length()]; 
    fill(_data, _data+array_length(), 0);
}

BitSet::BitSet(const BitSet &b)
{
    _size = b._size;
    _data = new size_t[array_length()];
    copy(b._data,b._data+array_length(),_data);
}

BitSet::~BitSet()
{
    delete[] _data;
}

BitSet &
BitSet::operator=(const BitSet &b)
{
    if (&b != this)
	{
	    delete[] _data;
	    _size = b._size;
	    _data = new size_t[array_length()];
	    copy(b._data,b._data+array_length(),_data);
	}
    return *this;
}


// -- COMPARISONS -------------------------------------------------------
bool
BitSet::operator==(const BitSet &b) const
{
    if (_size != b._size) return false;
    return equal(_data,_data+array_length(),b._data);
}

bool
BitSet::operator< (const BitSet &b) const
{
    return lexicographical_compare(_data,_data+array_length(),
				   b._data,b._data+b.array_length());
}


// -- SET OPERATIONS ----------------------------------------------------
namespace {
    // stl only defines _logical_ && and ||, we need _bit_ & and |.
    template<typename T> struct binary_or : public binary_function<T,T,T> {
	explicit binary_or() {};
	T operator()(T t1, T t2) const { return t1 | t2; }
    };
    template<typename T> struct binary_and : public binary_function<T,T,T> {
	explicit binary_and() {};
	T operator()(T t1, T t2) const { return t1 & t2; }
    };
};

BitSet &
BitSet::operator|=(const BitSet &b)
{
    assert(_size == b._size);
    transform(_data,_data+array_length(),b._data, _data, binary_or<size_t>());
    return *this;
}

BitSet &
BitSet::operator&=(const BitSet &b)
{
    assert(_size == b._size);
    transform(_data,_data+array_length(),b._data, _data, binary_and<size_t>());
    return *this;
}


// --- for debugging, mainly ------------------------------------------
void
BitSet::print(std::ostream &os) const
{
    os << '[';
    for (size_t i = 0; i < _size; ++i)
	os << (*this)[i];
    os << ']';
}

