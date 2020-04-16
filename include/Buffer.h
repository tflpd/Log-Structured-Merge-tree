//
// Created by Athanasios Filippidis on 2/25/20.
//

#ifndef LSM_TREE_BUFFER_H
#define LSM_TREE_BUFFER_H

#include "Tuple.h"
#include "Args.h"
#include <vector>

class Buffer{
private:
    std::vector<Tuple*> tuples;
    int _max_cap;
public:
    Buffer(int size);
    ~Buffer();

    bool Clear();
    //bool Get(Buffer* other); // move data from other buffer to this buffer
    std::vector<Tuple*> GetTuples();

    bool Append(std::string key, Value val);
    bool IsFull() const;

    void print() const;

    bool Scan(const Range& userAskedRange, Range& searchRange, 
        std::vector<Tuple*>& ret, std::vector<bool>& checkbits);
};

// #include "Buffer.cpp"
#endif //LSM_TREE_BUFFER_H
