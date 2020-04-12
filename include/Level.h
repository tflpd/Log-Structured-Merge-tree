//
// Created by Chengjun Wu on 3/01/20.
//

#ifndef LSM_TREE_LEVEL_H
#define LSM_TREE_LEVEL_H

#include <vector>
#include "Run.h"
#include "Parameters.h"
#include "Buffer.h"

class Level {
public:
    Level(uint level_id);
    ~Level();

    /// Takes the provided tuples, sorts them, creates a new run with them and adds that run to the level
    bool AddNewRun(vector<Tuple*>& tuples);
    /// Takes the vector tuples and adds to it all the tuples of the runs of the below level to this vector
    bool _AddMergeRuns(vector<Tuple*>& tuples);

    bool ReadyMerge() const;

private:
    bool _Clear();
    // Sorts the provided vector of tuples
    void _Sort(vector<Tuple*>& tuples);

private:
    uint _id;
    //uint _max_runs; // maximum runs allowed before merging
    uint _curr_tuples_n; // amount of tuples currently on this level
    uint _files_per_run; // This is not necessary since we keep in the Parameters struct the tuple size and the sst size.
    // More specifically this can be calculated as files_per_run = level0_max_tuples * pow(size_ratio, level_id) / _sst_size
    std::vector<Run> _runs;
};

#endif
