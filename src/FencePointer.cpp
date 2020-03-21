//
// Created by Athanasios Filippidis on 3/21/20.
//

#include "FencePointer.h"

FencePointer::FencePointer(int Interval_size): _interval_size(Interval_size){
}

FencePointer::~FencePointer() {

}

// Returns the index(from which then the offset can be calculated) of the key "key" in this file. If does not exist -1 OR
// Returns the (file) index of the key "key" in this level. If does not exist -1
// In order to get the offset the returned result needs to be multiplied with the pre set interval size
// TODO: This may be somehow optimized in the future to avoid the two ifs
int FencePointer::GetIndex(const char *key) {
    for (int i = 0; i < _fences.size(); ++i) {
        // TODO: Discuss about key's type and conversion
        int fence_number = atoi(_fences[i].c_str());
        if (atoi(key) <= fence_number) {
            if (i == _fences.size() - 1){
                return i*_interval_size;
            }
            if (key < _fences[i])
                return (i - 1)*_interval_size;
        }
    }
    return -1;
}

bool FencePointer::ClearFences() {
    _fences.clear();
    return true;
}

bool FencePointer::AddFence(std::string key) {
    _fences.push_back(key);
    return true;
}

int FencePointer::getIntervalSize() const {
    return _interval_size;
}