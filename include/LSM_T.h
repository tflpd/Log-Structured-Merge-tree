//
// Created by Chengjun Wu on 3/01/20.
//

#ifndef LSM_TREE_LSMT_H
#define LSM_TREE_LSMT_H

#include "Tuple.h"
#include "Level.h"
#include "Buffer.h"
#include <list>
#include "Database.hpp"

class LSM_T {
public:
    LSM_T(uint SST_size, uint a, uint runs);
    ~LSM_T();

    Tuple* Search(string key);
    Tuple* Search(string start, string end);
    bool Insert(string key, templatedb::Value val);
    bool Delete(string key);

private:
    std::list<Level> _levels;
    Buffer* _buf = nullptr;
    /* used for saving temporary data obtained after each merge, reducing one disk IO */
    //Buffer* _auxiliary_buf = nullptr;
    uint _sst_size;
    uint _a; // buf size = _a * _sst_size;
    uint _runs;
};

#endif
