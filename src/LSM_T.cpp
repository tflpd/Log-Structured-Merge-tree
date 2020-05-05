#include <assert.h>
#include <utility>
#include "LSM_T.h"
#include "Buffer.h"
#include "Tuple.h"
#include "Level.h"
#include "Log.h"
#include "Args.h"


LSM_T::LSM_T(int SSTSize, int tupleSize, int a, int runs):
        _sst_size(SSTSize), _tuple_size(tupleSize), _a(a), _max_runs_before_merging(runs) {

    assert(runs > 1 && a > 0);
    int bufsize = _a * _sst_size / _tuple_size;
    _buf = new Buffer(bufsize);
}

bool LSM_T::Insert(int key, Value val) {
    auto ret = _buf->Append(key, val);

    // if (_buf->IsFull()) {
    if (!ret) {
        // DEBUG_LOG("In-memory buffer is filled out!");
        auto tuples = _buf->GetTuples();
        // During the sort merge we want to have always on th leftmost part
        // the latest values
        reverse(tuples.begin(), tuples.end());
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
            DEBUG_LOG("creating a new level ...");
            auto lv = _levels.size();
            // TODO: Probably this should be uncommented to actually create the new level
            //Parameters arg(_tuple_size, _sst_size, _a, _max_runs_before_merging);
            _levels.emplace_back(lv);
            auto& bottom = _levels.back();
            // DEBUG_LOG(" new level is now appending a new run ...");
            bottom.AddNewRun(tuples);
        }


        // check if memory leak
        _buf->Clear();
        auto retRedo = _buf->Append(key, val);
        if (!retRedo) {
            DEBUG_LOG(std::string(" re-append key#") + to_string(key) + " to buffer failed!");
            return false;
        }

        DEBUG_LOG(std::string(" append key#") + to_string(key) + " to buffer success!");
        _buf->print();
    } else {
        DEBUG_LOG(std::string(" append key#") + to_string(key) + " to buffer success!");
        _buf->print();
    }

    DEBUG_LOG("");
    return true;
}

bool LSM_T::Delete(int key) {
    Value del_marker({TERMINATION});
    return Insert(key, del_marker);
}


void LSM_T::ShowMemBuffer() const {
    _buf->print();
}


Tuple* LSM_T::Search(int key) {
    return nullptr;
}

void re_order(std::vector<Tuple*>& tmpret, std::vector<Tuple*>& ret) {
    for (auto ptr : tmpret) {
        if (ptr != nullptr && !ptr->IsDelMarker()) 
            ret.push_back(ptr);
    }
}

void LSM_T::Search(int start, int end, std::vector<Tuple*>& ret) {
    std::string startlog = "new Search start: in range - " + std::to_string(start) 
        + ", " + std::to_string(end);
    DEBUG_LOG(startlog);
    int size = end - start + 1;

    Range* userAskedRange = new Range(start, end);
    Range* searchRange = new Range(start, end);

    std::vector<bool> checkbits(size, false);
    std::vector<Tuple*> tmpret(size, nullptr);

    bool finished = _buf->Scan(*userAskedRange, *searchRange, tmpret, checkbits);
    if (finished) {
        re_order(tmpret, ret);
        delete userAskedRange;
        delete searchRange;
        return;
    }

    for (auto level = _levels.begin(); level != _levels.end(); level++) {
        finished = level->Scan(*userAskedRange, *searchRange, tmpret, checkbits);
        if (finished)
            break;
    }

    re_order(tmpret, ret);
    delete userAskedRange;
    delete searchRange;    
}

LSM_T::~LSM_T() {}
