#include "Run.h"

FileMetaData::FileMetaData(FILE *File_pointer, const vector<Tuple*>& tuples):
        _file_pointer(File_pointer) {

    _num_tuples = tuples.size();
    _fence_pointerf = new FencePointer(500);

    // TODO: Write the values in the SST file and delete them from memory

    // Add a fence pointer every 500 (default value) keys
    addFences(tuples);

    // Add Bloom Filters of size 1024 and precision of 10 (bit) (default values) for the keys of the tuples
    addBloomFilters(tuples, 1024, 10);
}

FileMetaData::FileMetaData(FILE *File_pointer, const vector<Tuple*>& tuples, int FP_offset_interval, int BF_num_elements, int BF_bits_per_element):
        _file_pointer(File_pointer) {

    _num_tuples = tuples.size();
    _fence_pointerf = new FencePointer(FP_offset_interval);

    // TODO: Write the values in the SST file and delete them from memory

    // Add a fence pointer every FP_offset_interval (default value) keys
    addFences(tuples);

    // Add Bloom Filters of size BF_num_elements and precision of BF_bits_per_element (bit) for the keys of the tuples
    addBloomFilters(tuples, BF_num_elements, BF_bits_per_element);
}

bool FileMetaData::ModifyComponentsPostMerge(const vector<Tuple*>& tuples) {
    _num_tuples = tuples.size();
    _fence_pointerf = new FencePointer(500);

    // TODO: Write the values in the SST file using the already existing file pointer and delete them from memory

    // Add a fence pointer every 500 (default value) keys
    addFences(tuples);

    // Add Bloom Filters of size 1024 and precision of 10 (bit) (default values) for the keys of the tuples
    addBloomFilters(tuples, 1024, 10);
    return true;
}

bool FileMetaData::ModifyComponentsPostMerge(const vector<Tuple*>& tuples, int FP_offset_interval, int BF_num_elements,
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


void FileMetaData::addFences(vector<Tuple*> tuples){
    for (int i = 0; i < tuples.size() ; i += _fence_pointerf->getIntervalSize()) {
        _fence_pointerf->AddFence(tuples.at(i)->GetKey());
    }
    _fence_pointerf->AddFence(tuples.at(tuples.size() - 1)->GetKey());
}

void FileMetaData::addBloomFilters(vector<Tuple*> tuples, int BF_num_elements, int BF_bits_per_element) {
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
    return vector<Tuple*>();
}

FILE *FileMetaData::getFilePointer() const {
    return _file_pointer;
}

int FileMetaData::getNumTuples() const {
    return _num_tuples;
}














// Divides the provided tuples in files based on the provided parameters, creates that files and puts them there
Run::Run(uint files_per_run, vector<Tuple*> tuples, int Level_id, int Run_id, const Parameters& par):
        _files_per_run(files_per_run), _level_id(Level_id), _run_id(Run_id) {
    _num_tuples = tuples.size();
    uint files_to_be_created = _num_tuples * par.getTupleByteSize() / par.getSstSize();
    uint tuples_per_file = par.getSstSize() / par.getTupleByteSize();
    if (_num_tuples * par.getTupleByteSize() % par.getSstSize())
        files_to_be_created++;

    for (int i = 0; i < files_to_be_created; ++i) {
        // Taking the pointers that will be used to create the subvector which will be needed to
        // be passed as a parameter for the creation of the file
        // TODO: Possibly this can get optimized
        auto first = tuples.begin() + i*tuples_per_file;
        auto last = tuples.begin() + (i + 1)*tuples_per_file;
        if ((i + 1)*tuples_per_file > _num_tuples)
            last = tuples.begin() + _num_tuples - i*tuples_per_file;
        vector<Tuple*> newTmpVec(first, last);

        if (!AddNewFMD(newTmpVec, _level_id, _run_id))
            exit(-1);
    }
}

Run::~Run() {
    for (int i = 0; i < _files.size(); ++i) {
        delete(_files.at(i));
    }
}

//bool Run::Flush(Buffer* buf) {
//    return true;
//}
//
///* Collecting all binary data for this Run on disk
// * and re-org them in struct of Buffer */
//Buffer* Run::Fetch() {
//    for (auto& block : _files) {
//
//
//
//    }
//
//    return nullptr;
//
//}

bool Run::AddNewFMD(vector<Tuple*> tuples, int level_id, int run_id) {
    // TODO: Create SST file here and open it. Make sure we use _files.size() to get the serial id \
    //  Let's use a naming convention for the files like level3run5file2.sst and let's start \
    //  the counting from zero so the first level where the buffer gets dumped the first time next time will be level0 \
    // and the next time we create a new level this will be level1 so the lower level will be level 0 and the highest \
    // will be the n-th. Also when we merge some runs of a level let's reuse the same files. Deleting them and creating new
    // ones will probably lead to unnecessary overhead

//    if (_files.size() == _fp_per_run)
//        return false;
    FILE *fp = nullptr; // open fp here please
    auto *tmpFMD = new FileMetaData(fp, tuples);
    _files.push_back(tmpFMD);
    return true;
}

vector<Tuple*> Run::GetAllTuples() {
    uint size = 0;
    for (auto & _file : _files) {
        size += _file->GetAllTuples().size();
    }

    vector<Tuple*> result;
    result.reserve(size);
    // TODO: Maybe there is a better way to copy everything in results?
    for (auto & _file : _files) {
        result.insert(result.end(), _file->GetAllTuples().begin(), _file->GetAllTuples().end());
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