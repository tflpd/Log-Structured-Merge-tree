//
// Created by Chengjun Wu on 3/01/20.
//

#ifndef LSM_TREE_LEVEL_H
#define LSM_TREE_LEVEL_H

#include <vector>
#include "Run.h"
#include "Buffer.h"

class Level {
public:
    Level(uint id, uint runs);
    ~Level();

    bool Merge(Buffer* auxiliary_buffer);
    // Run* Merge(Run* merged_run);
    bool AppendRun(Buffer* auxiliary_buffer);
    // bool AppendRun(Run* merged_run);

    bool ReadyMerge() const;

private:
    bool _Clear();
    Buffer* _Sort(std::vector<Buffer*>& buf_vec);

private:
    uint _id;
    uint _max_runs; // maximum runs allowed before merging
    // uint _run_cnt; // current runs got
    uint _fp_per_run;
    std::vector<Run> _runs;

    Run* _placeholder = nullptr;
};

#include "Level.cpp"
#endif
