//
// Created by Athanasios Filippidis on 3/21/20.
//
#include <stdlib.h>
#include <vector>
#include "Args.h"
#include "FencePointerBeta.h"
#include "Tuple.h"

int FencePointerBeta::GetMin() const {
    return _fences.front().min;
}

int FencePointerBeta::GetMax() const {
    return _fences.back().min;
}

int FencePointerBeta::GetOffset(const char *key, int& start, int& end) {
    int iKey = atoi(key);
    for (auto& fence : _fences) {
        if (fence.min <= iKey && fence.max >= iKey) {
            start = fence.offset;
            end = fence.offset + fence.byteSize;
            return 0;
        }
    }

    // set an err case to trigger return stmt at upper layer
    start = 0;
    end = -1;
    return -1;
}

// one premise is that, the tuples are already sorted
void FencePointerBeta::SetupFence(const std::vector<Tuple*>& tuples) {
    if (!tuples.empty()) {
        int offset = 0; // offset at the file for each iteration
        auto lastIt = tuples.cend() - 1;

        for (std::vector<Tuple*>::const_iterator it = tuples.cbegin(); 
            it < tuples.cend(); it += _interval_size) {

            auto first = it;
            auto second = (it + _interval_size - 1 >= tuples.cend()) ? it + _interval_size - 1 : lastIt;

            _fences.emplace_back(first, second, offset);
            offset += _interval_size * getTupleBytesSize();
        }
    }
}

int FencePointerBeta::getIntervalSize() const {
    return _interval_size;
}

// void FencePointer::printFences() {
    
// }
