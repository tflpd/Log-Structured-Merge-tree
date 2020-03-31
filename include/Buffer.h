//
// Created by Athanasios Filippidis on 2/25/20.
//

#ifndef LSM_TREE_BUFFER_H
#define LSM_TREE_BUFFER_H

#include "Tuple.h"
#include "Database.hpp"

#define SIZE 10

class Buffer{
private:
    //Tuple *tuples;
    std::vector<Tuple*> *tuples;
public:
    Buffer(int size);
    ~Buffer();

    bool Clear();
    //bool Get(Buffer* other); // move data from other buffer to this buffer
    std::vector<Tuple*> *GetTuples();

    bool Append(std::string key, templatedb::Value val);
    bool IsFull() const;
};

#endif //LSM_TREE_BUFFER_H
