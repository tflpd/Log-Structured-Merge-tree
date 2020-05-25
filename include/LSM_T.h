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
    LSM_T(int minSSTSize, int tupleSize, int a, int runs);
    ~LSM_T();

    Tuple* Search(int key);
    void Search(int start, int end, vector<Tuple*>& ret);
    bool Insert(int key, Value val);
    bool Delete(int key);

    void ShowMemBuffer() const;

private:
    std::list<Level*> _levels;
    Buffer* _buf = nullptr;
    /* used for saving temporary data obtained after each merge, reducing one disk IO */
    int _tuple_size;
    int _sst_size; /// Size of an sst file in BYTES
    int _a; // buf size = _a * _sst_size;
    int _max_runs_before_merging;
};

// #include "LSM_T.cpp"
#endif
