//
// Created by Athanasios Filippidis on 3/24/20.
//

#ifndef TEMPLATEDB_PARAMETERS_H
#define TEMPLATEDB_PARAMETERS_H

#include <iostream>

class Parameters {
public:

    Parameters(uint tupleByteSize, uint sstSize, uint a, uint maxMergeRuns) : 
        _tuple_byte_size(tupleByteSize), _sst_size(sstSize), _a(a), _max_merge_runs(maxMergeRuns){}

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

    uint getMaxMergeRuns() const {
        return _max_merge_runs;
    }

    uint getTuplesLevel0() const {
        return _tuples_level0;
    }

    uint getFencePointersIntervals() const {
        return _fence_pointers_intervals;
    }

    uint getBfNumElements() const {
        return _BF_num_elements;
    }

    uint getBfPrecision() const {
        return _BF_precision;
    }

    uint getBufferTuplesSize() const {
        return _buffer_tuples_size;
    }

    uint getSizeRatio() const {
        return _size_ratio;
    }

private:
    /// Tuple related parameters
    uint _tuple_byte_size;
    /// SST files related parameters
    uint _sst_size;
    /// Buffer related parameters
    uint _a; // buffer size = _a * _sst_size;
    uint _buffer_tuples_size; // how many tuples does the buffer can fit
    /// Level related parameters. Also we can take the derivative of some those for a more fine granularity
    uint _max_merge_runs; // maximum runs allowed in a level before merging - this should be either one for leveling or equal to max level capacity for tiering
    uint _tuples_level0; // maximum tuples in level zero - recursively every next level will have _size_ratio the amount of the previous
    uint _size_ratio; // So for example level one runs will be equal to size ratio * level0_runs or equally level one tuples will be equal to size ratio * level0_tuples
    /// Fence pointers related parameters
    uint _fence_pointers_intervals;
    /// Bloom filters related parameteres
    uint _BF_num_elements;
    uint _BF_precision; // expressed in bits per element
};

#endif //TEMPLATEDB_PARAMETERS_H
