// Copyright (C) 2003, 2004 by BiRC -- Bioinformatics Research Center
//                             University of Aarhus, Denmark
//                             Contact: Thomas Mailund <mailund@birc.dk>

#include "bitset.hh"
#include <cassert>
using namespace std;

int
main()
{
    BitSet b1(5);
    for(int i = 0; i < 5; ++i)
	assert(b1[i] == false);

    b1[0] = true;
    assert(b1[0] == true);
    for(int i = 1; i < 5; ++i)
	assert(b1[i] == false);

    BitSet b2(5);
    b2[4] = true;
    assert(b2[4] == true);
    for (int i = 0; i < 4; ++i)
	assert(b2[i] == false);

    // INV: b1=10000, b2=00001

    BitSet b3(b2);
    for (int i = 0; i < 5; ++i)
	assert(b3[i] == b2[i]);
    assert(b3 == b2);
    assert(b3 != b1);

    BitSet b4(5);
    b4 = b1;
    for (int i = 0; i < 5; ++i)
	assert(b4[i] == b1[i]);
    assert(b4 == b1);
    assert(b4 != b2);

    // INV: b1=b4=10000, b2=b3=00001

    assert(~b4[1]);
    assert(~b3[3]);
    b4[4] = b3[4];
    b3[0] = b4[0];
    assert(b4[4] == true);
    assert(b3[0] == true);

    b4[4] = b3[2];
    b3[0] = b4[2];
    assert(~b4[4]);
    assert(~b3[0]);

    assert(b1 == b4);
    assert(b2 == b3);

    b1[2] = true;
    b2[1] = true;

    // INV: b1=10100, b2=01001, b3=00001, b4=10000

    BitSet b5(b1 | b2);

    assert( b1[0]);
    assert(!b1[1]);
    assert( b1[2]);
    assert(!b1[3]);
    assert(!b1[4]);

    assert(!b2[0]);
    assert( b2[1]);
    assert(!b2[2]);
    assert(!b2[3]);
    assert( b2[4]);

    assert( b5[0]);
    assert( b5[1]);
    assert( b5[2]);
    assert(!b5[3]);
    assert( b5[4]);

    b4 |= b1;
    assert( b4[0]);
    assert(!b4[1]);
    assert( b4[2]);
    assert(!b4[3]);
    assert(!b4[4]);

    // INV: b1=10100, b2=01001, b3=00001, b4=10100, b5=11101

    b5 = b2 & b3;
    assert(!b5[0]);
    assert(!b5[1]);
    assert(!b5[2]);
    assert(!b5[3]);
    assert( b5[4]);

    // INV: b1=10100, b2=01001, b3=00001, b4=10100, b5=00001

    b4 |= b2;
    assert( b4[0]);
    assert( b4[1]);
    assert( b4[2]);
    assert(!b4[3]);
    assert( b4[4]);

    // INV: b1=10100, b2=01001, b3=00001, b4=11101, b5=00001

    b4 &= b2;
    assert(b2 == b4);

    // INV: b1=10100, b2=b4=01001, b3=00001, b5=00001

    assert(b2 < b1);
    assert(b3 < b2);
    assert(b3 < b1);

}
