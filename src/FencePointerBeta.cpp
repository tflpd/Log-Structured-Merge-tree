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
    return _fences.back().max;
}

int FencePointerBeta::GetOffset(const char *key, int& start, int& end) {
    int iKey = atoi(key);
    start = 0; end = -1; // set an err case to trigger return stmt at upper layer
    if (_fences.size() == 0) return -1;

    int index = _fences.size() - 1;
    while (index >= 0 && _fences[index].max >= iKey) 
        index--;

    int farMostQualifiedIndex = index + 1;
    if (farMostQualifiedIndex < 0 || farMostQualifiedIndex >= _fences.size()) return -1;

    start = _fences[farMostQualifiedIndex].offset;
    end = _fences[farMostQualifiedIndex].offset + _fences[farMostQualifiedIndex].byteSize;
    return 0;
}

// one premise is that, the tuples are already sorted
void FencePointerBeta::SetupFence(const std::vector<Tuple*>& tuples) {
    if (!tuples.empty()) {
        int offset = 0; // offset at the file for each iteration
        auto lastIt = tuples.cend() - 1;

        for (std::vector<Tuple*>::const_iterator it = tuples.cbegin(); 
            it < tuples.cend(); it += _interval_size) {

            auto first = it;
            auto second = (it + _interval_size - 1 >= tuples.cend()) ? lastIt : it + _interval_size - 1;

            _fences.emplace_back(first, second, offset);
            offset += _interval_size * getTupleBytesSize();
        }
    }
}

int FencePointerBeta::getIntervalSize() const {
    return _interval_size;
}

void FencePointerBeta::printFences() const {
    // for (int index = 0; index < _fences.size(); index++) {
    //     auto fence = _fences[index];
    //     std::cout << "#" << index << " fence from (" << fence.min << "to " << fence.max << ")"; 
    //     std::cout << " total byte length is " << fence.byteSize;
    //     std::cout << " offset starts at " << fence.offset << std::endl;
    // }
}
