#include "./include/Level.h"
#include <vector>
#define MEMORY_CAP 10800


Level::Level(uint id, uint runs): _id(id), _max_runs(runs) {
    // calculate _fp_per_run
}

Level::~Level() {

}

bool Level::ReadyMerge() const {
    return _max_runs == _runs.size() + 1;
}

/* the data to be pushed down is able to fit into memory */
bool Level::Merge(Buffer* auxiliary_buffer) {
    std::vector<Buffer*> buf_vec;

    // collect data across all Runs
    for (auto& run : _runs) {
        auto buf = run.Fetch();
        buf_vec.push_back(buf);
    }

    // merge-sort
    auto sorted_buf = _Sort(buf_vec);

    // release memory in case of memory leak
    for (auto p_buf : buf_vec)
        delete p_buf;

    auxiliary_buffer->Clear();
    _Clear();
    return true;
}

bool Level::AppendRun(Buffer* auxiliary_buffer) {
    _runs.emplace_back(_fp_per_run);
    auto last = _runs.back();
    last.Flush(auxiliary_buffer);

    auxiliary_buffer->Clear();
    return true;
}

bool Level::_Clear() {
    std::vector<Run> tmp;
    _runs.swap(tmp);
    return true;
}

Buffer* Level::_Sort(std::vector<Buffer*>& buf_vec) {
    return nullptr;
}

/* the data to be pushed down is too large to fit into memory */
// Run* Level::Merge(Run* merged_run) {

// }

// bool Level::AppendRun(Run* merged_run) {

// }
