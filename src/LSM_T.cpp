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

bool LSM_T::Insert(std::string key, Value val) {
    auto ret = _buf->Append(key, val);

    // if (_buf->IsFull()) {
    if (!ret) {
        // DEBUG_LOG("In-memory buffer is filled out!");
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
            DEBUG_LOG(std::string(" re-append key#") + key + " to buffer failed!");
            return false;
        }

        DEBUG_LOG(std::string(" append key#") + key + " to buffer success!");
        _buf->print();
    } else {
        DEBUG_LOG(std::string(" append key#") + key + " to buffer success!");
        _buf->print();
    }

    DEBUG_LOG("");
    return true;
}

bool LSM_T::Delete(std::string key) {
    // auto ret = Search(key);
    // if (ret == nullptr) {
    //     // may need to release ret before exit
    //     // delete ret;
    //     return false;
    // }

    // return Insert(key, DEL_PLACEHOLDER);
    return true;
}


void LSM_T::ShowMemBuffer() const {
    _buf->print();
}


Tuple* LSM_T::Search(std::string key) {

    return nullptr;
}

void re_order(std::vector<Tuple*>& tmpret, std::vector<Tuple*>& ret) {
    for (auto ptr : tmpret)
        if (ptr != nullptr) 
            ret.push_back(ptr);
}

void LSM_T::Search(std::string start, std::string end, std::vector<Tuple*>& ret) {
    int iStart = std::stoi(start), iEnd = std::stoi(end);
    int size = iEnd - iStart + 1;

    Range* userAskedRange = new Range(iStart, iEnd);
    Range* searchRange = new Range(iStart, iEnd);

    std::vector<bool> checkbits(size, false);
    std::vector<Tuple*> tmpret(size, nullptr);

    bool finished = _buf->Scan(*searchRange, tmpret, checkbits);
    if (finished) {
        re_order(tmpret, ret);
        delete userAskedRange;
        delete searchRange;
        return;
    }

    // if not finished, re-init lookup range
    ShrinkSearchRange(*userAskedRange, *searchRange, checkbits);
    std::cout << "first shrinked range:" << std::endl;
    PrintRange(*searchRange);


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
