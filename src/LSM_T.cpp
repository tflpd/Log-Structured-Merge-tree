#include <assert.h>
#include "LSM_T.h"
#include "Buffer.h"
#include "Tuple.h"
#include "Level.h"
#ifdef __linux__
#include <bits/stdc++.h>
#endif

#define DEL_PLACEHOLDER INT_MIN
#define AUX_MULTIPLIER 8

LSM_T::LSM_T(uint SST_size, uint a, uint runs):
        _sst_size(SST_size), _a(a), _runs(runs) {

    assert(runs > 1 && a > 0);
    _buf = new Buffer(_a * _sst_size);
    _auxiliary_buf = new Buffer(AUX_MULTIPLIER * _a * _sst_size);
}

bool LSM_T::Insert(int key, int val) {
    _buf->Append(key, val);

    if (_buf->IsFull()) {

        bool push_down = true;
        _auxiliary_buf->Get(_buf); // move data at _buf to _auxiliary_buf

        for (auto it = _levels.begin(); it != _levels.end() && push_down; it++) {

            if (it->ReadyMerge())
                it->_AddMergeRuns(_auxiliary_buf);
            else {
                push_down = false;
                it->AddNewRun(_auxiliary_buf->GetTuples()); // create a new Run
            }

        }

        // touch the bottom of levels, create a new Level
        if (push_down) {
            auto cnt = _levels.size();
            //_levels.emplace_back(++cnt, _runs);

            auto bottom = _levels.back();
            bottom.AddNewRun(_auxiliary_buf->GetTuples());
        }
    }

    return true;
}

bool LSM_T::Delete(int key) {
    auto ret = Search(key);
    if (ret == nullptr) {
        // may need to release ret before exit
        // delete ret;
        return false;
    }

    return Insert(key, DEL_PLACEHOLDER);
}


Tuple* LSM_T::Search(int key) {

    return nullptr;
}

Tuple* LSM_T::Search(int start, int end) {

    return nullptr;
}

LSM_T::~LSM_T() {}
