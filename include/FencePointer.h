//
// Created by Athanasios Filippidis on 3/21/20.
//

#ifndef TEMPLATEDB_FENCEPOINTER_H
#define TEMPLATEDB_FENCEPOINTER_H

#include <iostream>
#include <vector>

// Fence pointer for finding offsets of keys inside the sst files OR
// Fence pointer for finding files indexes of keys in a level

class FencePointer {
public:
    FencePointer(int Interval_size);
    virtual ~FencePointer();
    // Returns the index(from which then the offset can be calculated) of the key "key" in this file. If does not exist -1 OR
    // Returns the (file) index of the key "key" in this level. If does not exist -1
    int GetIndex(const char *key);
    bool AddFence(std::string key);
    bool ClearFences();

    int getIntervalSize() const;

    void printFences();

    int GetMin() const;
    int GetMax() const;


private:
    std::vector<std::string> _fences;
    // This describes actually the "frequency" of fence pointers
    // So for example, if set to 2, every two keys we will have a fence pointer
    // which will point out the smaller and the largest key value of those two keys OR
    // So for example, if set to 2, every two files we will have a fence pointer
    // which will point out the smaller and the largest key value of those two files
    int _interval_size;
};


#endif //TEMPLATEDB_FENCEPOINTER_H
