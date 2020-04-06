#include "Run.h"
#include "Log.h"
#include "Args.h"
#include <fstream>
#include <stdio.h>
// #include <fcntl.h>
// #include <unistd.h>

FileMetaData::FileMetaData(FILE *File_pointer, const vector<Tuple*> tuples, std::string FileName):
        _file_pointer(File_pointer), _file_name(FileName) {

    _num_tuples = tuples.size();
    _fence_pointerf = new FencePointer(500);

    // TODO: Write the values in the SST file and delete them from memory
    int tupleByteSize = getTupleSize();
    char* wbuf = new char[_num_tuples*tupleByteSize];
    // std::vector<char> wbuf(_num_tuples*tupleByteSize, 0);

    for (int i = 0; i < _num_tuples; i++) {
        int offset = i * tupleByteSize;
        auto p_tuple = tuples[i];
        p_tuple->AppendBin2Vec(wbuf + offset);
    }
    
    // built-in C buffering method
    // can be optimized by writing our own buffer
    // setvbuf(FILE *restrict stream, char *restrict buf, int type, size_t size)
    fwrite(wbuf, 1, _num_tuples*tupleByteSize, File_pointer);

    // Add a fence pointer every 500 (default value) keys
    addFences(tuples);

    // Add Bloom Filters of size 1024 and precision of 10 (bit) (default values) for the keys of the tuples
    addBloomFilters(tuples, 1024, 10);


    delete[] wbuf;
}

FileMetaData::FileMetaData(std::string FileName) : _file_name(FileName) {}

FileMetaData::FileMetaData(FILE *File_pointer, const vector<Tuple*> tuples, int FP_offset_interval, int BF_num_elements, 
        int BF_bits_per_element, std::string FileName):
        _file_pointer(File_pointer), _file_name(FileName) {

    _num_tuples = tuples.size();
    _fence_pointerf = new FencePointer(FP_offset_interval);

    int tupleByteSize = getTupleSize();
    char* wbuf = new char[_num_tuples*tupleByteSize];
    // std::vector<char> wbuf(_num_tuples*tupleByteSize, 0);

    for (int i = 0; i < _num_tuples; i++) {
        int offset = i * tupleByteSize;
        auto p_tuple = tuples[i];
        p_tuple->AppendBin2Vec(wbuf + offset);
    }
    
    // built-in C buffering method
    // can be optimized by writing our own buffer
    // setvbuf(FILE *restrict stream, char *restrict buf, int type, size_t size)
    fwrite(wbuf, 1, _num_tuples*tupleByteSize, File_pointer);

    // Add a fence pointer every FP_offset_interval (default value) keys
    addFences(tuples);

    // Add Bloom Filters of size BF_num_elements and precision of BF_bits_per_element (bit) for the keys of the tuples
    addBloomFilters(tuples, BF_num_elements, BF_bits_per_element);

    // TODO: need to write fence ptr & bloomfilter to files as well
    // TODO: either delete tuples here or out at the Run

    delete[] wbuf;
}

bool FileMetaData::ModifyComponentsPostMerge(const vector<Tuple*> tuples) {
    _num_tuples = tuples.size();
    _fence_pointerf = new FencePointer(500);

    // TODO: Write the values in the SST file using the already existing file pointer and delete them from memory

    // Add a fence pointer every 500 (default value) keys
    addFences(tuples);

    // Add Bloom Filters of size 1024 and precision of 10 (bit) (default values) for the keys of the tuples
    addBloomFilters(tuples, 1024, 10);
    return true;
}

bool FileMetaData::ModifyComponentsPostMerge(const vector<Tuple*> tuples, int FP_offset_interval, int BF_num_elements,
                                             int BF_bits_per_element) {
    _num_tuples = tuples.size();
    _fence_pointerf = new FencePointer(FP_offset_interval);

    // TODO: Write the values in the SST file using the already existing file pointer and delete them from memory

    // Add a fence pointer every FP_offset_interval (default value) keys
    addFences(tuples);

    // Add Bloom Filters of size BF_num_elements and precision of BF_bits_per_element (bit) for the keys of the tuples
    addBloomFilters(tuples, BF_num_elements, BF_bits_per_element);
    return true;
}

// TODO: Check what resources need to be de allocated
FileMetaData::~FileMetaData() {
    delete _fence_pointerf;
}

//void FileMetaData::flush() {
//
//}

std::string FileMetaData::getFileName() const {
    return _file_name;
}

void FileMetaData::addFences(const vector<Tuple*> tuples){
    for (int i = 0; i < tuples.size() ; i += _fence_pointerf->getIntervalSize()) {
        _fence_pointerf->AddFence(tuples.at(i)->GetKey());
    }
    _fence_pointerf->AddFence(tuples.at(tuples.size() - 1)->GetKey());
}

void FileMetaData::addBloomFilters(const vector<Tuple*> tuples, int BF_num_elements, int BF_bits_per_element) {
    //assert(tuples.size() >= BF_num_elements && tuples.size()%BF_num_elements == 0);

    auto *tmpBF = new BF::BloomFilter(BF_num_elements, BF_bits_per_element);
    _bloom_filters.push_back(tmpBF);
    for (int i = 1; i < tuples.size()/BF_num_elements; ++i) {
        tmpBF = new BF::BloomFilter(BF_num_elements, BF_bits_per_element);
        _bloom_filters.push_back(tmpBF);
    }

    int j = -1;
    for (int i = 0; i < tuples.size(); ++i) {
        if (i%BF_num_elements == 0)
            j++;
        _bloom_filters.at(j)->program(tuples.at(i)->GetKey());
    }
}

// TODO: This should open the file pointer, retrieve all the tuples in it and return them
vector<Tuple*> FileMetaData::GetAllTuples() {
    vector<Tuple*> ret;
    int tupleSize = getTupleSize();
    char* tmpbuf = new char[tupleSize*_num_tuples];

    FILE* fp = fopen(_file_name.c_str(), "rb");
    fread(tmpbuf, 1, tupleSize * _num_tuples, fp);
    
    for (int num = 0; num < _num_tuples; num++) {
        int offset = num * tupleSize;
        auto p_tuple = new Tuple();
        p_tuple->Read2Tuple(tmpbuf + offset);

        ret.push_back(p_tuple);
    }

    fclose(fp);
    // for (auto p_tuple : ret)
    //     cout << p_tuple->ToString() << endl;

    delete[] tmpbuf;
    return ret;
}

FILE *FileMetaData::getFilePointer() const {
    return _file_pointer;
}

int FileMetaData::getNumTuples() const {
    return _num_tuples;
}

// Divides the provided tuples in files based on the provided parameters, creates that files and puts them there
Run::Run(uint files_per_run, vector<Tuple*>& tuples, int Level_id, int Run_id, const Parameters par):
        _files_per_run(files_per_run), _level_id(Level_id), _run_id(Run_id) {
    _num_tuples = tuples.size();

    uint files_to_be_created = _num_tuples * par.getTupleByteSize() / par.getSstSize();
    uint tuples_per_file = par.getSstSize() / par.getTupleByteSize();
    if (_num_tuples * par.getTupleByteSize() % par.getSstSize())
        files_to_be_created++;

    DEBUG_LOG(std::string("Constructing Run#") + std::to_string(_run_id) + 
        ": creating #" + std::to_string(files_to_be_created) + 
        " files with #" + std::to_string(tuples_per_file) + " tuples per file."); 

    for (int i = 0; i < files_to_be_created; ++i) {
        // Taking the pointers that will be used to create the subvector which will be needed to
        // be passed as a parameter for the creation of the file
        // TODO: Possibly this can get optimized
        auto first = tuples.begin() + i*tuples_per_file;
        auto last = tuples.begin() + (i + 1)*tuples_per_file;
        if ((i + 1)*tuples_per_file > _num_tuples)
            last = tuples.begin() + _num_tuples - i*tuples_per_file;

        vector<Tuple*> newTmpVec(first, last);

        if (!AddNewFMD(newTmpVec, _level_id, _run_id)) {
            KEY_LOG(std::string("Constructing Run#)") + std::to_string(_run_id) + 
                "failed! Because AddNewFMD failed.");
            exit(-1);
        }
    }
}

Run::~Run() {
    for (int i = 0; i < _files.size(); ++i) {
        delete(_files.at(i));
    }
}

bool Run::DeleteFMD() {
    for (auto pFile : _files) {
        const char* fileName = pFile->getFileName().c_str();
        remove(fileName); // system call
    }

    return true;
}

bool Run::AddNewFMD(vector<Tuple*>& tuples, int level_id, int run_id) {
    // TODO: Create SST file here and open it. Make sure we use _files.size() to get the serial id \
    //  Let's use a naming convention for the files like level3run5file2.sst and let's start \
    //  the counting from zero so the first level where the buffer gets dumped the first time next time will be level0 \
    // and the next time we create a new level this will be level1 so the lower level will be level 0 and the highest \
    // will be the n-th. Also when we merge some runs of a level let's reuse the same files. Deleting them and creating new
    // ones will probably lead to unnecessary overhead
    
    int fdSerialID = _files.size();
    std::string fileName("level" + std::to_string(level_id) + 
        "run" + std::to_string(run_id) + 
        "file" + std::to_string(fdSerialID) + ".sst");
    DEBUG_LOG(std::string("starting add new FMD of file#") + fileName);

    FILE* fp = fopen(fileName.c_str(), "wb");
    auto *tmpFMD = new FileMetaData(fp, tuples, fileName);
    fclose(fp);

    _files.push_back(tmpFMD);
    return true;
}

vector<Tuple*> Run::GetAllTuples() {
    vector<Tuple*> result;
    // TODO: Maybe there is a better way to copy everything in results?
    for (auto & _file : _files) {
        DEBUG_LOG(std::string("getting tuples from #") + _file->getFileName());
        auto tuples = _file->GetAllTuples();
        result.insert(result.end(), tuples.begin(), tuples.end());
    }
    return result;
}

// A very complex thought to avoid recreating files when we merge some runs in a single run let's not thinks about it yet
//bool Run::ModifyFMDsComponents(uint files_per_run, const vector<Tuple> &tuples, const Parameters &_par) {
//    _num_tuples = tuples.size();
//    uint existing_file_num = _num_tuples * _par.getTupleByteSize() / _par.getSstSize();
//    uint tuples_per_file = _par.getSstSize() / _par.getTupleByteSize();
//    if (_num_tuples * _par.getTupleByteSize() % _par.getSstSize())
//        existing_file_num++;
//
//    for (int i = 0; i < existing_file_num; ++i) {
//        // Taking the pointers that will be used to create the subvector which will be needed to
//        // be passed as a parameter for the creatio of the file
//        // TODO: Possibly this can get optimized
//        auto first = tuples.begin() + i*tuples_per_file;
//        auto last = tuples.begin() + (i + 1)*tuples_per_file;
//        if ((i + 1)*tuples_per_file > _num_tuples)
//            last = tuples.begin() + _num_tuples - i*tuples_per_file;
//        vector<Tuple> newTmpVec(first, last);
//
//        if (!_files.at(i)->ModifyComponentsPostMerge(newTmpVec))
//            exit(-1);
//    }
//    return true;
//}
