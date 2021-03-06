//
// Created by Athanasios Filippidis on 3/21/20.
//

#include "FencePointer.h"

FencePointer::FencePointer(int Interval_size): _interval_size(Interval_size){
}

FencePointer::~FencePointer() {

}

int FencePointer::GetMin() const {
    return atoi(_fences.front().c_str());
}

int FencePointer::GetMax() const {
    return atoi(_fences.back().c_str());
}

// Returns the index(from which then the offset can be calculated) of the key "key" in this file. If does not exist -1 OR
// Returns the (file) index of the key "key" in this level. If does not exist -1
// In order to get the offset the returned result needs to be multiplied with the pre set interval size
// TODO: This may be somehow optimized in the future to avoid the two ifs
int FencePointer::GetIndex(const char *key, int& start, int& end, int final_segment_size) {
    int uint_key = atoi(key);
    start = end = -1;
    for (int i = 0; i < _fences.size(); ++i) {
        // TODO: Discuss about key's type and conversion
        int fence_number = atoi(_fences[i].c_str());
        //std::cout << "bout to compare " << std::to_string(uint_key) << " with " << std::to_string(fence_number) << std::endl;
        if (uint_key <= fence_number) {
            if (uint_key < fence_number){
                start = (i - 1)*_interval_size;
                /// If this fence pointer is previous to the last
                if (i == _fences.size() - 2){
                    /// The plus one is to actually move the pointer to the end of the tuple
                    end = start + final_segment_size + 1;
                }else{
                    /// The plus one is to actually move the pointer to the end of the tuple
                    end = start + _interval_size + 1;
                }
                return start;
            }

            if (uint_key == fence_number && i == _fences.size() - 1){
                start = i*_interval_size;
                end = start + 1;
                return start;
            }
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

void FencePointer::printFences() {
    //std::cout << "Printing fence pointers: ..." << std::endl;
    for (int i = 0; i < _fences.size(); ++i) {
        DEBUG_LOG(std::string("Printing fence pointer: ... ") + std::string(_fences.at(i)));
        //std::cout << _fences.at(i) << std::endl;
    }
}
