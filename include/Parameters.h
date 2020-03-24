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
    uint _max_merge_runs; // maximum runs allowed in a level before merging
    uint _max_runs_level0; // maximum runs in level zero - recursively every next level will have double the amount of the previous
    uint _fence_pointers_intervals;
    uint _BF_num_elements;
    uint _BF_precision; // expressed in bits per element
};

#endif //TEMPLATEDB_PARAMETERS_H
