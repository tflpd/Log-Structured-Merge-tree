//
// Created by Chengjun Wu on 3/01/20.
//

#ifndef LSM_TREE_RUN_H
#define LSM_TREE_RUN_H

#include "Fence_Ptr.h"
#include "Buffer.h"
#include <vector>

struct Block
{
    FencePointer fp;
    FileMetaData meta_data;

    Block();
    ~Block();

    void flush();
    void fetch() const;
};

class Run {
public:
    Run(uint fps);
    ~Run(); // release resources held by Block?

    bool Flush(Buffer* buf);
    // bool Fetch() const;
    Buffer* Fetch();

private:
    std::vector<Block> _blocks; // can be optimized to pre-defined sized array
    uint _fp_per_run;
};

#endif