//
// Created by Chengjun Wu on 3/01/20.
//

#ifndef LSM_TREE_RUN_H
#define LSM_TREE_RUN_H

#include "FencePointer.h"
#include "Buffer.h"
#include "BloomFilter.h"
#include "Database.hpp"
#include "Parameters.h"
#include <vector>

struct FileMetaData
{
public:
    // Default constructor
    FileMetaData(FILE *File_pointer, const vector<Tuple*>& tuples);
    // Constructor with parameterizable fence pointers intervals and bloom filter values capacity and precision
    FileMetaData(FILE *File_pointer, const vector<Tuple*>& tuples, int FP_offset_interval, int BF_num_elements,
                 int BF_bits_per_element);

    ~FileMetaData();

    //void flush();
    vector<Tuple*> GetAllTuples();
    FILE *getFilePointer() const;
    // Those two functions are symmetric to the constructors - they are just to be used to modify
    // the components of the FMD after merging some runs - in order to avoid creating new FMD's for
    // each run we can just re-use the existing ones
    bool ModifyComponentsPostMerge(const vector<Tuple*>& tuples);
    bool ModifyComponentsPostMerge(const vector<Tuple*>& tuples, int FP_offset_interval, int BF_num_elements,
                                   int BF_bits_per_element);

    int getNumTuples() const;

private:
    FILE *_file_pointer;
    FencePointer *_fence_pointerf;
    vector<BF::BloomFilter*> _bloom_filters;
    // The number of the tuples in this file
    int _num_tuples;

    // Adds/creates the fences of this file consuming the provided tuples
    void addFences(vector<Tuple*> tuples);
    // Adds/creates the BFs (with specific parameters) of this file consuming the provided tuples
    void addBloomFilters(vector<Tuple*> tuples, int BF_num_elements, int BF_bits_per_element);
};










class Run {
public:
    Run(uint files_per_run, vector<Tuple*> tuples, int level_id, int run_id, const Parameters& par);
    ~Run(); // release resources held by FileMetaData?

    //bool Flush(Buffer* buf);
    // bool Fetch() const;
    //Buffer* Fetch();

    // Adds/creates a new file and the respective FileMetaData struct of this file.
    // This file will consist of the tuples "tuples". The two id's passed will be used for the file creation/naming
    // Returns true if created correctly false if this run is full of files and a new run should be created
    bool AddNewFMD(vector<Tuple*> tuples, int level_id, int run_id);
    // A very complex thought to avoid recreating files when we merge some runs in a single run let's not thinks about it yet
    //bool ModifyFMDsComponents(uint files_per_run, const vector<Tuple>& tuples, const Parameters& _par);
    // Accumulates all the tuples of the underlying files of this run and returns them
    vector<Tuple*> GetAllTuples();

private:
    std::vector<FileMetaData*> _files; // can be optimized to pre-defined sized array
    int _files_per_run;
    uint _num_tuples;
    int _level_id;
    int _run_id;
};

#endif