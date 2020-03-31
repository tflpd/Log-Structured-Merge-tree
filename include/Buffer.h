//
// Created by Athanasios Filippidis on 2/25/20.
//

#ifndef LSM_TREE_BUFFER_H
#define LSM_TREE_BUFFER_H

#include "Tuple.h"
#include <vector>

class Buffer{
private:
    std::vector<Tuple> tuples;
    int _max_cap;
public:
    Buffer(int size);
    ~Buffer();

    bool Clear();
    bool Get(Buffer* other); // move data from other buffer to this buffer

    bool Append(std::string key, Value val);
    bool IsFull() const;

    void print() const;
};

// #include "Buffer.cpp"
#endif //LSM_TREE_BUFFER_H
