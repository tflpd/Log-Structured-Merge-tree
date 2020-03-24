//
// Created by Athanasios Filippidis on 3/24/20.
//

#ifndef TEMPLATEDB_PARAMETERS_H
#define TEMPLATEDB_PARAMETERS_H

#include <iostream>

class Parameters {
public:

    Parameters(uint tupleByteSize, uint sstSize, uint a) : _tuple_byte_size(tupleByteSize), _sst_size(sstSize), _a(a) {}

    virtual ~Parameters() {

    }

    uint getTupleByteSize() const {
        return _tuple_byte_size;
    }

    uint getSstSize() const {
        return _sst_size;
    }

    uint getA() const {
        return _a;
    }

private:
    uint _tuple_byte_size;
    uint _sst_size;
    uint _a; // buffer size = _a * _sst_size;
};

#endif //TEMPLATEDB_PARAMETERS_H
