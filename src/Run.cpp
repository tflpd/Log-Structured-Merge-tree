#include "./include/Run.h"


Run::Run(uint fps): _fp_per_run(fps) {

}

Run::~Run() {

}

bool Run::Flush(Buffer* buf) {

}

/* Collecting all binary data for this Run on disk
 * and re-org them in struct of Buffer */
Buffer* Run::Fetch() {
    for (auto& block : _blocks) {



    }

}

Block::Block() {

}

Block::~Block() {

}

void Block::flush() {

}

void Block::fetch() const {

}


// struct Block
// {
// 	FencePointer fp;
// 	FileMetaData meta_data;

// 	void flush() const;
// 	void fetch() const;
// };

// class Run {
// public:
// 	Run(uint fps);
// 	~Run(); // release resources held by Block?

// 	bool Flush(Buffer* buf);
// 	// bool Fetch() const;
// 	Buffer* Fetch();

// private:
// 	std::vector<Block> _blocks; // can be optimized to pre-defined sized array
// 	uint _fp_per_run;
// };
