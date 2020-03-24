#include <assert.h>
#include <utility>
#include "LSM_T.h"
#include "Buffer.h"
#include "Tuple.h"
#include "Level.h"
#include "Log.h"
#ifdef __linux__
#include <bits/stdc++.h>
#endif

#define DEL_PLACEHOLDER INT_MIN

LSM_T::LSM_T(uint SST_size, uint a, uint runs):
        _sst_size(SST_size), _a(a), _runs(runs) {

    assert(runs > 1 && a > 0);
    _buf = new Buffer(_a * _sst_size);
}

bool LSM_T::Insert(std::string key, Value val) {
    _buf->Append(key, val);

    DEBUG_LOG("Start insertion ...");

    if (_buf->IsFull()) {
        DEBUG_LOG("In-memory buffer is filled out!");
        auto tuples = _buf->GetTuples();
        bool push_down = true;

        int cnt = 0;
        for (auto it = _levels.begin(); it != _levels.end() && push_down; it++) {

            if (it->ReadyMerge()) {
                DEBUG_LOG(std::string("lv#") + std::to_string(cnt) + " start to merge ...");
                it->_AddMergeRuns(tuples);
            }
            else {
                DEBUG_LOG(std::string("lv#") + std::to_string(cnt) + " append a new run ...");
                push_down = false;
                it->AddNewRun(tuples); // create a new Run
            }

            cnt++;
        }

        // touch the bottom of levels, create a new Level
        if (push_down) {
            DEBUG_LOG("creating a new run ...");
            auto cnt = _levels.size();
            // TODO: Probably this should be uncommented to actually create the new level
            Parameters arg(100, _sst_size, _a, _runs);
            _levels.emplace_back(cnt, arg);

            auto bottom = _levels.back();
            DEBUG_LOG(" new lv is now appending a new run ...");
            bottom.AddNewRun(tuples);
        }

    }

    DEBUG_LOG("Insertion ends...");
    return true;
}

bool LSM_T::Delete(std::string key) {
    auto ret = Search(key);
    if (ret == nullptr) {
        // may need to release ret before exit
        // delete ret;
        return false;
    }

    return Insert(key, DEL_PLACEHOLDER);
}


void LSM_T::ShowMemBuffer() const {
    _buf->print();
}


Tuple* LSM_T::Search(std::string key) {

    return nullptr;
}

Tuple* LSM_T::Search(std::string start, std::string end) {

    return nullptr;
}

LSM_T::~LSM_T() {}
