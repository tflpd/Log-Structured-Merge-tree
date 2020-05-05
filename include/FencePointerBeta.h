//
// Created by Athanasios Filippidis on 3/21/20.
//

#ifndef TEMPLATEDB_FENCEPOINTERBETA_H
#define TEMPLATEDB_FENCEPOINTERBETA_H

#include <iostream>
#include <vector>
#include "Log.h"
#include "Args.h"
#include "Tuple.h"

// Fence pointer for finding offsets of keys inside the sst files OR
// Fence pointer for finding files indexes of keys in a level
struct Fence {
    int min;
    int max;
    int byteSize;
    int tupleCnts;
    int offset;

    Fence(std::vector<Tuple*>::const_iterator first, std::vector<Tuple*>::const_iterator second, int byteStart) : offset(byteStart){
        tupleCnts = second - first + 1;
        byteSize = tupleCnts * getTupleBytesSize();
        min = (*first)->GetKey();
        max = (*second)->GetKey();
    }
};

class FencePointerBeta {
public:
    FencePointerBeta(int Interval_size) : _interval_size(Interval_size) {}
    ~FencePointerBeta() {}
    // Returns the index(from which then the offset can be calculated) of the key "key" in this file. If does not exist -1 OR
    // Returns the (file) index of the key "key" in this level. If does not exist -1
    int GetOffset(const char *key, int& start, int& end);
    int GetMin() const;
    int GetMax() const;
    int getIntervalSize() const;

    void SetupFence(const std::vector<Tuple*>& tuples);
    // void printFences();
private:
    std::vector<Fence> _fences;
    int _interval_size;
};


#endif //TEMPLATEDB_FENCEPOINTER_H
