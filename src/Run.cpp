#include "Run.h"

FileMetaData::FileMetaData(FILE *File_pointer, const vector<Tuple>& tuples):
        _file_pointer(File_pointer) {

    _num_tuples = tuples.size();
    _fence_pointerf = new FencePointer(500);

    // TODO: Write the values in the SST file

    // Add a fence pointer every 500 (default value) keys
    addFences(tuples);

    // Add Bloom Filters of size 1024 and precision of 10 (bit) (default values) for the keys of the tuples
    addBloomFilters(tuples, 1024, 10);
}

FileMetaData::FileMetaData(FILE *File_pointer, const vector<Tuple>& tuples, int FP_offset_interval, int BF_num_elements, int BF_bits_per_element):
        _file_pointer(File_pointer) {

    _num_tuples = tuples.size();
    _fence_pointerf = new FencePointer(FP_offset_interval);

    // TODO: Write the values in the SST file

    // Add a fence pointer every 500 (default value) keys
    addFences(tuples);

    // Add Bloom Filters of size 1024 (default value) for the keys of the tuples
    addBloomFilters(tuples, BF_num_elements, BF_bits_per_element);
}

// TODO: Check what resources need to be de allocated
FileMetaData::~FileMetaData() {
    delete _fence_pointerf;
}

void FileMetaData::flush() {

}


void FileMetaData::addFences(vector<Tuple> tuples){
    for (int i = 0; i < tuples.size() ; i += _fence_pointerf->getIntervalSize()) {
        _fence_pointerf->AddFence(tuples.at(i).GetKey());
    }
    _fence_pointerf->AddFence(tuples.at(tuples.size() - 1).GetKey());
}

void FileMetaData::addBloomFilters(vector<Tuple> tuples, int BF_num_elements, int BF_bits_per_element) {
    //assert(tuples.size() >= BF_num_elements && tuples.size()%BF_num_elements == 0);

    auto *tmpBF = new BF::BloomFilter(BF_num_elements, BF_bits_per_element);
    _bloom_filters.push_back(tmpBF);
    for (int i = 1; i < tuples.size()/BF_num_elements; ++i) {
        auto *tmpBF = new BF::BloomFilter(BF_num_elements, BF_bits_per_element);
        _bloom_filters.push_back(tmpBF);
    }

    int j = -1;
    for (int i = 0; i < tuples.size(); ++i) {
        if (i%BF_num_elements == 0)
            j++;
        _bloom_filters.at(j)->program(tuples.at(i).GetKey());
    }
}

// TODO: This should open the file pointer, retrieve all the tuples in it and return them
vector<Tuple> FileMetaData::GetAllTuples() {
    return vector<Tuple>();
}


Run::Run(uint fps): _fp_per_run(fps) {

}

Run::~Run() {
    for (int i = 0; i < _files.size(); ++i) {
        delete(_files.at(i));
    }
}

bool Run::Flush(Buffer* buf) {
    return true;
}

/* Collecting all binary data for this Run on disk
 * and re-org them in struct of Buffer */
Buffer* Run::Fetch() {
    for (auto& block : _files) {



    }

    return nullptr;

}

bool Run::AddNewFMD(const vector<Tuple>& tuples, int level_id, int run_id) {
    // TODO: Create SST file here and open it
//    if (_files.size() == _fp_per_run)
//        return false;
    FILE *fp = nullptr;
    auto *tmpFMD = new FileMetaData(fp, tuples);
    _files.push_back(tmpFMD);
    return true;
}

vector<Tuple> Run::GetALlTuples() {
    uint size = 0;
    for (auto & _file : _files) {
        size += _file->GetAllTuples().size();
    }

    vector<Tuple> result;
    result.reserve(size);
    // TODO: Maybe there is a better way to copy everything in results?
    for (auto & _file : _files) {
        result.insert(result.end(), _file->GetAllTuples().begin(), _file->GetAllTuples().end());
    }
    return result;
}


// struct FileMetaData
// {
// 	FencePointer file_pointer;
// 	FileMetaData meta_data;

// 	void flush() const;
// 	void fetch() const;
// };

// class Run {
// public:
// 	Run(uint fps);
// 	~Run(); // release resources held by FileMetaData?

// 	bool Flush(Buffer* buf);
// 	// bool Fetch() const;
// 	Buffer* Fetch();

// private:
// 	std::vector<FileMetaData> _files; // can be optimized to pre-defined sized array
// 	uint _fp_per_run;
// };
