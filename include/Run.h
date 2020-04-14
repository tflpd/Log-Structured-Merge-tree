//
// Created by Chengjun Wu on 3/01/20.
//

#ifndef LSM_TREE_RUN_H
#define LSM_TREE_RUN_H

#include "FencePointer.h"
#include "Buffer.h"
#include "BloomFilter.h"
#include "Tuple.h"
#include "Parameters.h"
#include "Args.h"
#include <vector>

struct FileMetaData
{
public:
    // Default constructor
    FileMetaData(FILE *File_pointer, const vector<Tuple*> tuples, std::string FileName);

    // will be called from failure/reboot recovery
    FileMetaData(std::string FileName);

    // Constructor with parameterizable fence pointers intervals and bloom filter values capacity and precision
//    FileMetaData(FILE *File_pointer, const vector<Tuple*> tuples, int FP_offset_interval, int BF_num_elements,
//                 int BF_bits_per_element, std::string FileName);

    ~FileMetaData();

    //void flush();
    vector<Tuple*> GetAllTuples();
    FILE *getFilePointer() const;
    // Those two functions are symmetric to the constructors - they are just to be used to modify
    // the components of the FMD after merging some runs - in order to avoid creating new FMD's for
    // each run we can just re-use the existing ones
    bool ModifyComponentsPostMerge(const vector<Tuple*> tuples);
    bool ModifyComponentsPostMerge(const vector<Tuple*> tuples, int FP_offset_interval, int BF_num_elements,
                                   int BF_bits_per_element);

    int getNumTuples() const;
    std::string getFileName() const;
    void printFences();
    int getTupleOffset(const char *key);

    void Collect(const Range& userAskedRange, Range& searchRange, 
        std::vector<Tuple*>& ret, std::vector<bool>& checkbits);

private:
    FILE *_file_pointer;
    std::string _file_name;
    FencePointer *_fence_pointerf;
    vector<BF::BloomFilter*> _bloom_filters;
    // The number of the tuples in this file
    int _num_tuples;

    // Adds/creates the fences of this file consuming the provided tuples
    void addFences(const vector<Tuple*>& tuples);
    // Adds/creates the BFs (with specific parameters) of this file consuming the provided tuples
    void addBloomFilters(const vector<Tuple*> tuples, int BF_num_elements, int BF_bits_per_element);

    void fastBFIndex(const Range& userAskedRange, Range& searchRange, 
        std::vector<Tuple*>& ret, std::vector<bool>& checkbits, Range& suggestRange);
    void fastFPIndex(const Range& userAskedRange, Range& suggestRange, 
        std::vector<Tuple*>& ret, std::vector<bool>& checkbits, Range& offset);
};




class Run {
public:
    // Run(uint f, vector<Tuple*>& tuples, int lv_id, int run_id);
    Run(uint files_per_run, vector<Tuple*>& tuples, int level_id, int run_id);
    ~Run(); // release resources held by FileMetaData?

    void GenerateFileName(char* name, int level_id, int run_id);

    // Adds/creates a new file and the respective FileMetaData struct of this file.
    // This file will consist of the tuples "tuples". The two id's passed will be used for the file creation/naming
    // Returns true if created correctly false if this run is full of files and a new run should be created
    bool AddNewFMD(vector<Tuple*>& tuples, int level_id, int run_id);
    // A very complex thought to avoid recreating files when we merge some runs in a single run let's not thinks about it yet
    //bool ModifyFMDsComponents(uint files_per_run, const vector<Tuple>& tuples, const Parameters& _par);
    // Accumulates all the tuples of the underlying files of this run and returns them
    vector<Tuple*> GetAllTuples();

    bool DeleteFMD(); // shouldn't put on deconstructor as deconstructor will be called once after program ends
    bool Scan(const Range& userAskedRange, Range& searchRange, 
        std::vector<Tuple*>& ret, std::vector<bool>& checkbits);
    int getTupleOffset(const char *key);

private:
    std::vector<FileMetaData*> _files; // can be optimized to pre-defined sized array
    int _files_per_run;
    uint _num_tuples;
    int _level_id;
    int _run_id;
};

#endif
