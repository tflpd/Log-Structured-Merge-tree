//
// Created by Chengjun Wu on 3/01/20.
//

#ifndef LSM_TREE_LSMT_H
#define LSM_TREE_LSMT_H

#include "Tuple.h"
#include "Level.h"
#include "Buffer.h"
#include <list>

class LSM_T {
public:
    LSM_T(uint SST_size, uint a, uint runs);
    ~LSM_T();

    Tuple* Search(std::string key);
    Tuple* Search(std::string start, std::string end);
    bool Insert(std::string key, Value val);
    bool Delete(std::string key);

private:
    std::list<Level> _levels;
    Buffer* _buf = nullptr;
    /* used for saving temporary data obtained after each merge, reducing one disk IO */
    Buffer* _auxiliary_buf = nullptr;
    uint _sst_size;
    uint _a; // buf size = _a * _sst_size;
    uint _runs;
};

#include "LSM_T.cpp"
#endif
