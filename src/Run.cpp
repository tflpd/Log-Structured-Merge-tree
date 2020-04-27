#include "Run.h"
#include "Log.h"
#include "Args.h"
#include <fstream>
#include <utility>
#include <stdio.h>
// #include <fcntl.h>
#include <unistd.h>

FileMetaData::FileMetaData(FILE *File_pointer, const vector<Tuple*>& tuples, std::string FileName):
        _file_pointer(File_pointer), _file_name(std::move(FileName)) {

    _num_tuples = tuples.size();
    _fence_pointerf = new FencePointer(getFPInterval());

    // TODO: Write the values in the SST file and delete them from memory
    int tupleByteSize = getTupleBytesSize();
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

    // Add a fence pointer every FP_INTERV keys
    addFences(tuples);

    // Add Bloom Filters of size BF_NUM_TUPLES and precision of BF_BITS_PER_ELEMENT (bit) for the keys of the tuples
    addBloomFilters(tuples, getBFNumTuples(), getBFBitsPerElement());

    // TODO: need to write fence ptr & bloomfilter to files as well
    // TODO: either delete tuples here or out at the Run


    delete[] wbuf;
}

//FileMetaData::FileMetaData(std::string FileName) : _file_name(FileName) {}

//FileMetaData::FileMetaData(FILE *File_pointer, const vector<Tuple*> tuples, int FP_offset_interval, int BF_num_elements,
//        int BF_bits_per_element, std::string FileName):
//        _file_pointer(File_pointer), _file_name(FileName) {
//
//    _num_tuples = tuples.size();
//    _fence_pointerf = new FencePointer(FP_offset_interval);
//
//    int tupleByteSize = getTupleBytesSize();
//    char* wbuf = new char[_num_tuples*tupleByteSize];
//    // std::vector<char> wbuf(_num_tuples*tupleByteSize, 0);
//
//    for (int i = 0; i < _num_tuples; i++) {
//        int offset = i * tupleByteSize;
//        auto p_tuple = tuples[i];
//        p_tuple->AppendBin2Vec(wbuf + offset);
//    }
//
//    // built-in C buffering method
//    // can be optimized by writing our own buffer
//    // setvbuf(FILE *restrict stream, char *restrict buf, int type, size_t size)
//    fwrite(wbuf, 1, _num_tuples*tupleByteSize, File_pointer);
//
//    // Add a fence pointer every FP_offset_interval (default value) keys
//    addFences(tuples);
//
//    // Add Bloom Filters of size BF_num_elements and precision of BF_bits_per_element (bit) for the keys of the tuples
//    addBloomFilters(tuples, BF_num_elements, BF_bits_per_element);
//
//    // TODO: need to write fence ptr & bloomfilter to files as well
//    // TODO: either delete tuples here or out at the Run
//
//    delete[] wbuf;
//}

//bool FileMetaData::ModifyComponentsPostMerge(const vector<Tuple*> tuples) {
//    _num_tuples = tuples.size();
//    _fence_pointerf = new FencePointer(500);
//
//    // TODO: Write the values in the SST file using the already existing file pointer and delete them from memory
//
//    // Add a fence pointer every 500 (default value) keys
//    addFences(tuples);
//
//    // Add Bloom Filters of size 1024 and precision of 10 (bit) (default values) for the keys of the tuples
//    addBloomFilters(tuples, 1024, 10);
//    return true;
//}
//
//bool FileMetaData::ModifyComponentsPostMerge(const vector<Tuple*> tuples, int FP_offset_interval, int BF_num_elements,
//                                             int BF_bits_per_element) {
//    _num_tuples = tuples.size();
//    _fence_pointerf = new FencePointer(FP_offset_interval);
//
//    // TODO: Write the values in the SST file using the already existing file pointer and delete them from memory
//
//    // Add a fence pointer every FP_offset_interval (default value) keys
//    addFences(tuples);
//
//    // Add Bloom Filters of size BF_num_elements and precision of BF_bits_per_element (bit) for the keys of the tuples
//    addBloomFilters(tuples, BF_num_elements, BF_bits_per_element);
//    return true;
//}

// TODO: Check what resources need to be de allocated
FileMetaData::~FileMetaData() {
    delete _fence_pointerf;
    for (auto & _bloom_filter : _bloom_filters) {
        delete _bloom_filter;
    }
}

void FileMetaData::fastBFIndex(const Range& userAskedRange, Range& searchRange, 
        std::vector<Tuple*>& ret, std::vector<bool>& checkbits, Range& suggestRange) {
    auto it = checkbits.begin();
    auto pBF = _bloom_filters.at(0);
    std::string l = _file_name + " -> The search range is: [" + to_string(searchRange._begin) +
        ", " + to_string(searchRange._end) + "]";
    DEBUG_LOG(l);

    int startIndex = searchRange._begin - userAskedRange._begin;
    int endIndex = searchRange._end - userAskedRange._begin;
    int len = endIndex - startIndex + 1;
    int startpoint = searchRange._begin, endpoint = searchRange._end;

    // can be refined, no need to do two passes
    for (int index = startIndex; index <= endIndex; index++) {
        int queryKey = userAskedRange._begin + index; 
        std::string sKey = std::to_string(queryKey);

        if (!checkbits.at(index) &&
            pBF->query(sKey)) {
            startpoint = queryKey;
            break;
        }
    }

    for (int index = endIndex; index >= startIndex; index--) {
        int queryKey = userAskedRange._begin + index;
        std::string sKey = std::to_string(queryKey);

        if (!checkbits.at(index) &&
            pBF->query(sKey)) {
            endpoint = queryKey;
            break;
        }
    }

    suggestRange._begin = startpoint;
    suggestRange._end = endpoint;
    std::string log = _file_name + " -> The suggested min key of BF is: #" + to_string(startpoint) +
        ", the suggested max key of BF is: #" + to_string(endpoint);

    DEBUG_LOG(log);
}

void FileMetaData::fastFPIndex(const Range& userAskedRange, Range& suggestRange, 
        std::vector<Tuple*>& ret, std::vector<bool>& checkbits, Range& offset) {

    // init with an invalid range that'd let system know this case
    // should be skipped
    int start = 0;
    int end = -1;

    int minKey = _fence_pointerf->GetMin();
    int maxKey = _fence_pointerf->GetMax();
    std::string log1 = _file_name + " -> The min key of FP is: #" + to_string(minKey) +
        ", the max key of this FP is: #" + to_string(maxKey);

    int startKey = (suggestRange._begin >= minKey) ? suggestRange._begin : minKey;  
    int endKey = (suggestRange._end <= maxKey) ? suggestRange._end : maxKey;
    std::string log2 = _file_name + " -> After doing intersection, the suggested start key of FP is: #" + to_string(startKey) +
        ", the suggested max key of this FP is: #" + to_string(endKey);

    DEBUG_LOG(log1);
    DEBUG_LOG(log2);

    // check if intersects
    if (startKey <= endKey) {
        const char* ssk = std::to_string(startKey).c_str();
        const char* sek = std::to_string(endKey).c_str();

        int offsetStart = getTupleOffset(ssk);
        int offsetEnd = getTupleOffset(sek);

        // shouldn't get -1 here, as we ensure they lie between [minKey, maxKey].
        // but make a KEYLOG here in case of any exceptions
        if (offsetStart != -1 && offsetEnd != -1) {
            start = offsetStart;
            // how many tuples for an interval * tuple per byte 
            end = offsetEnd + getFPInterval() * getTupleBytesSize();
        } else {
            std::string log = _file_name + " -> unexpected err occur when startKey = " + std::to_string(startKey) +
                " and endKey = " + std::to_string(endKey);
            KEY_LOG(log);
        }
    }

    offset._begin = start;
    offset._end = end;
    std::string log3 = _file_name + " -> Offset given by this FP is: [" + std::to_string(start) +
                " ," + std::to_string(end) + "]";
    DEBUG_LOG(log3);
}

void FileMetaData::Collect(const Range& userAskedRange, Range& searchRange,  
    std::vector<Tuple*>& ret, std::vector<bool>& checkbits) {
    std:string marker = "Current search Range is: [" + std::to_string(searchRange._begin) + ", "
        + std::to_string(searchRange._end) + "]";
    DEBUG_LOG(marker);
    Range suggestRange(0, 0);
    fastBFIndex(userAskedRange, searchRange, ret, checkbits, suggestRange);
    // need to do disk IO as BF ensure us there will be at least one value matches 
    // our needs
    // also, if read == true, startpoint is surely larger or equal to endpoint
    if (suggestRange._begin <= suggestRange._end) {
        Range offset(0, 0);
        fastFPIndex(userAskedRange, suggestRange, ret, checkbits, offset);

        int tupleSize = getTupleBytesSize();
        int start = offset._begin, end = offset._end + _fence_pointerf->getIntervalSize() * tupleSize;
        int size = end - start;

        if (size <= 0) return; // no satisfied data in this file

        FILE* fp = fopen(_file_name.c_str(), "rb");
        char* tmpbuf = new char[size];
        fseek(fp, start, SEEK_SET);
        fread(tmpbuf, 1, size, fp);
        fclose(fp);

        int _num_tuples = size / tupleSize;

        for (int num = 0; num < _num_tuples; num++) {
            int offset = num * tupleSize;
            auto p_tuple = new Tuple();
            p_tuple->Read2Tuple(tmpbuf + offset);
            int key = p_tuple->GetKey();

            if (key >= userAskedRange._begin && key <= userAskedRange._end
                && !checkbits[(key - userAskedRange._begin)]) {
                cout << "read from:" + _file_name + " the #" + std::to_string(num) + "tuple" << endl;
                cout << key << endl;
                cout << p_tuple->_value.items[0] << ", " << p_tuple->_value.items[1] << endl;

                checkbits[(key - userAskedRange._begin)] = true;
                ret[(key - userAskedRange._begin)] = p_tuple;
            } else
                delete p_tuple;
        }

    }
}   

std::string FileMetaData::getFileName() const {
    return _file_name;
}

/// It will always add at least two fence pointers, one to the start and one to the end of the passed tuples
/// Apart from that it will add one FP after FP_INTERV tuples
/// So for example for FP_INTERV = 2 it will add a pointer at position 0
/// and then the next one will be at position 3
void FileMetaData::addFences(const vector<Tuple*>& tuples){
    //std::cout << _fence_pointerf->getIntervalSize() << std::endl;
    //std::cout << tuples.size() << std::endl;
    for (int i = 0; i < tuples.size() ; i += _fence_pointerf->getIntervalSize() + 1) {
        _fence_pointerf->AddFence(std::to_string(
            tuples.at(i)->GetKey()));
        //std::cout << "MPIKA1" << std::endl;
    }
    if (tuples.size() - 1 <= _fence_pointerf->getIntervalSize() + 1){
        final_segment_size = 0;
    } else{
        final_segment_size = (tuples.size() - 1) % (_fence_pointerf->getIntervalSize() + 1);
        if (final_segment_size != 0){
            _fence_pointerf->AddFence(std::to_string(
                    tuples.back()->GetKey()));
            //std::cout << "MPIKA2" << std::endl;
        }
    }
}

void FileMetaData::addBloomFilters(const vector<Tuple*>& tuples, int BF_num_elements, int BF_bits_per_element) {
    //assert(tuples.size() >= BF_num_elements && tuples.size()%BF_num_elements == 0);
    auto *tmpBF = new BF::BloomFilter(BF_num_elements, BF_bits_per_element);
    _bloom_filters.push_back(tmpBF);
    /// If the number of BFs we need to create is even
    if (tuples.size() % BF_num_elements == 0){
        /// Then continue creating as many need after the first
        for (int i = 1; i < tuples.size()/BF_num_elements; ++i) {
            tmpBF = new BF::BloomFilter(BF_num_elements, BF_bits_per_element);
            _bloom_filters.push_back(tmpBF);
        }
    }else{
        /// If the number is odd and we need more than one
        if (tuples.size()/BF_num_elements >= 1){
            /// Add as many needed plus one (thus starting from 0) that will just not be full
            for (int i = 0; i < tuples.size()/BF_num_elements; ++i) {
                tmpBF = new BF::BloomFilter(BF_num_elements, BF_bits_per_element);
                _bloom_filters.push_back(tmpBF);
            }
        }
    }


    int j = -1;
    for (int i = 0; i < tuples.size(); ++i) {
        if (i%BF_num_elements == 0)
            j++;
        _bloom_filters.at(j)->program(std::to_string(
            tuples.at(i)->GetKey()));
    }
}

// TODO: This should open the file pointer, retrieve all the tuples in it and return them
vector<Tuple*> FileMetaData::GetAllTuples() {
    vector<Tuple*> ret;
    int tupleSize = getTupleBytesSize();
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

void FileMetaData::printFences() {
    _fence_pointerf->printFences();
}

/// Returns the offset of the tuple that is the fence right before the target tuple
/// So the key we are looking for should be max FP_INTERV away
/// If not found returns -1
int FileMetaData::getTupleOffset(const char *key, int& start, int& end) {
    //DEBUG_LOG(std::string("PRIN PRIN PERASA BF") + "");
    for (auto & _bloom_filter : _bloom_filters) {
        //DEBUG_LOG(std::string("PRIN PERASA BF") + "");
        if (_bloom_filter->query(key)){
            //DEBUG_LOG(std::string("PERASA BF") + "");
            int index = _fence_pointerf->GetIndex(key, start, end, final_segment_size);
            if (index >= 0){
                return index * getTupleBytesSize();
            }else{
                start = end = -1;
                return -1;
            }
        }
    }
    return -1;
}

// Divides the provided tuples in files based on the provided parameters, creates that files and puts them there
Run::Run(uint files_per_run, vector<Tuple*>& tuples, int Level_id, int Run_id):
        _files_per_run(files_per_run), _level_id(Level_id), _run_id(Run_id) {
    _num_tuples = tuples.size();

    uint files_to_be_created = _num_tuples * getTupleBytesSize() / getSSTSize();
    uint max_tuples_per_file = getSSTSize() / getTupleBytesSize();
    if (_num_tuples * getTupleBytesSize() % getSSTSize())
        files_to_be_created++;

    // DEBUG_LOG(std::string("Constructing Run#") + std::to_string(_run_id) +
    //                   " of Level#" + std::to_string(_level_id) +
    //                   ": creating #" + std::to_string(files_to_be_created) +
    //     " file(s) with up to #" + std::to_string(max_tuples_per_file) + " tuples per file.");

    for (int i = 0; i < files_to_be_created; ++i) {
        // Taking the pointers that will be used to create the subvector which will be needed to
        // be passed as a parameter for the creation of the file
        // TODO: Possibly this can get optimized
        auto first = tuples.begin() + i*max_tuples_per_file;
        auto last = tuples.begin() + (i + 1)*max_tuples_per_file;
        if ((i + 1)*max_tuples_per_file > _num_tuples)
            last = tuples.begin() + _num_tuples;

        vector<Tuple*> newTmpVec(first, last);

        if (!AddNewFMD(newTmpVec, _level_id, _run_id)) {
            KEY_LOG(std::string("Constructing Run#)") + std::to_string(_run_id) + 
                "failed! Because AddNewFMD failed.");
            exit(-1);
        }
    }
}

Run::~Run() {
    for (auto & _file : _files) {
        delete _file;
    }
}

bool Run::DeleteFMD() {
    for (auto pFile : _files) {
        const char* fileName = pFile->getFileName().c_str();

#ifdef __linux__
        unlink(fileName);
#else
        remove(fileName); // system call
#endif
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
    tmpFMD->printFences();
    fclose(fp);

    _files.push_back(tmpFMD);
    //DEBUG_LOG(std::string("Requesting byte offset for key 9") + std::to_string(getTupleOffset("9")));
    return true;
}

vector<Tuple*> Run::GetAllTuples() {
    vector<Tuple*> result;
    for (auto & _file : _files) {
        DEBUG_LOG(std::string("getting tuples from #") + _file->getFileName());
        auto tuples = _file->GetAllTuples();
        result.insert(result.end(), tuples.begin(), tuples.end());
    }
    return result;
}

bool Run::Scan(const Range& userAskedRange, Range& searchRange, 
        std::vector<Tuple*>& ret, std::vector<bool>& checkbits) {
    // due to the property of Run, it's safe to make an assertion that
    // no dupulicate keys exist in the same Run. we may only iterate through
    // FMD and  ask BF & FP in order to collect satisfied tuples
    for (auto pFmd : _files)
        pFmd->Collect(userAskedRange, searchRange, ret, checkbits);

    // check if all keys in [start, end] have been collected or not
    ShrinkSearchRange(userAskedRange, searchRange, checkbits);
    if (searchRange._begin > searchRange._end) return true;
    return false;
}

/// Returns the bytes offset of the tuple that is the fence right before the target tuple
/// So the key we are looking for should be max FP_INTERV away
/// If not found returns -1
//int Run::getTupleOffset(const char *key) {
//    for (int i = 0; i < _files.size(); ++i) {
//        int tuple_offset = _files.at(i)->getTupleOffset(key);
//        if (tuple_offset >= 0)
//            return tuple_offset;
//    }
//    return -1;
//}

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
