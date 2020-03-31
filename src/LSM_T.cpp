#include <assert.h>

#include <utility>
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
    //_auxiliary_buf = new Buffer(AUX_MULTIPLIER * _a * _sst_size);
}

bool LSM_T::Insert(string key, templatedb::Value val) {
    _buf->Append(std::move(key), std::move(val));

    if (_buf->IsFull()) {
        vector<Tuple*> *tuples;
        bool push_down = true;
        //_auxiliary_buf->Get(_buf); // move data at _buf to _auxiliary_buf
        tuples = _buf->GetTuples();

        for (auto it = _levels.begin(); it != _levels.end() && push_down; it++) {

            if (it->ReadyMerge())
                it->_AddMergeRuns(tuples);
            else {
                push_down = false;
                it->AddNewRun(*tuples); // create a new Run
            }

        }

        // touch the bottom of levels, create a new Level
        if (push_down) {
            auto cnt = _levels.size();
            // TODO: Probably this should be uncommented to actually create the new level
            //_levels.emplace_back(++cnt, _runs);

            auto bottom = _levels.back();
            bottom.AddNewRun(*tuples);
        }
    }

    return true;
}

bool LSM_T::Delete(string key) {
    auto ret = Search(key);
    if (ret == nullptr) {
        // may need to release ret before exit
        // delete ret;
        return false;
    }

    return Insert(key, DEL_PLACEHOLDER);
}


Tuple* LSM_T::Search(string key) {

    return nullptr;
}

Tuple* LSM_T::Search(string start, string end) {

    return nullptr;
}

LSM_T::~LSM_T() {}
