#include "Level.h"
#include <vector>
#include "Args.h"
#define MEMORY_CAP 10800


Level::Level(uint level_id):
        _id(level_id) {
    // calculate _fp_per_run
    //_files_per_run = _par.getTuplesLevel0() * pow(_par.getSizeRatio(), level_id)  / _par.getSstSize();
    _files_per_run = getBufferBytesSize() * getMaxRunsBeforeMerge() * pow(getLevelSizeRatio(), level_id)  / getSSTSize();
    if (_files_per_run == 0)
        _files_per_run++;
    _curr_tuples_n = 0;
}

Level::~Level() {

}

bool Level::ReadyMerge() const {
    return getMaxRunsBeforeMerge() == _runs.size() + 1;
}

/* the data to be pushed down is able to fit into memory */
// Takes the vector tuples and adds to it all the tuples of the runs of the below level to this vector
bool Level::_AddMergeRuns(vector<Tuple*>& tuples) {
    //std::vector<Tuple*> accumulative_tuples;
    // TODO: maybe change here the curr tuples counter
    //accumulative_tuples.reserve(final_run_size);

    // TODO: maybe in the future just pop the min first element of every run -> more optimal
    // Number of tuples on the resulting run
//    uint final_run_size = 0;
//    for (auto& run : _max_runs_before_merging) {
//        final_run_size += run.GetAllTuples().size();
//    }
//    final_run_size += tuples->size();
//
//    for (int i = 0; i < final_run_size; ++i) {
//        uint min_value = UINT_MAX;
//        uint min_run_index = _max_runs_before_merging.size() + 1;
//        for (auto& run : _max_runs_before_merging) {
//
//        }
//    }

    // Add all tuples of this level in a vector
    for (auto& run : _runs) {
        auto arr = run.GetAllTuples();
        tuples.insert(tuples.end(), arr.begin(), arr.end());
        //accumulative_tuples.insert(accumulative_tuples.end(), run.GetAllTuples().begin(), run.GetAllTuples().end());
    }
    // and the ones provided as an argument
    //accumulative_tuples.insert(accumulative_tuples.end(), tuples.begin(), tuples.end());

    // Merge-sort them
    //_Sort(&accumulative_tuples);

    // TODO: Delete all existing runs and their sst files here
//    for (auto p_buf : buf_vec)
//        delete p_buf;
    _Clear();

    return true;
}

//bool Level::AppendRun(Buffer* auxiliary_buffer) {
//    _max_runs_before_merging.emplace_back(_files_per_run);
//    auto last = _max_runs_before_merging.back();
//    last.Flush(auxiliary_buffer);
//
//    auxiliary_buffer->Clear();
//    return true;
//}

bool Level::_Clear() {
    for (auto& run : _runs) 
        run.DeleteFMD(); 

    std::vector<Run> tmp;
    _runs.swap(tmp);       
    return true;
}

// Merges two subarrays of tuples[].
// First subarray is tuples[left..mid]
// Second subarray is tuples[mid+1..right]
void merge(std::vector<Tuple*>& tuples, unsigned long left, unsigned long mid, unsigned long right)
{
    unsigned long i, j, k;
    unsigned long n1 = mid - left + 1;
    unsigned long n2 = right - mid;

    /* create temp arrays */
    std::vector<Tuple*> L;
    L.reserve(n1);
    std::vector<Tuple*> R;
    R.reserve(n2);

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = (tuples)[left + i];
    for (j = 0; j < n2; j++)
        R[j] = (tuples)[mid + 1 + j];

    /* Merge the temp arrays back into tuples[left..right]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = left; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (stoi(L[i]->GetKey()) <= stoi(R[j]->GetKey()))
        {
            (tuples)[k] = L[i];
            i++;
        }
        else
        {
            (tuples)[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        (tuples)[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        (tuples)[k] = R[j];
        j++;
        k++;
    }
}

/* left is for left index and right is right index of the
   sub-array of arr to be sorted */
void mergeSort(std::vector<Tuple*>& arr, unsigned long left, unsigned long right)
{
    if (left < right)
    {
        // Same as (left+right)/2, but avoids overflow for
        // large left and right
        unsigned long mid = left + (right - left) / 2;

        // Sort first and second halves
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

void Level::_Sort(vector<Tuple*>& tuples) {
    mergeSort(tuples, 0, tuples.size() - 1);
}


bool Level::AddNewRun(vector<Tuple*>& tuples) {
    _curr_tuples_n += tuples.size();
//    if (_max_runs_before_merging.size() == _par.getMaxMergeRuns()){
//        _AddMergeRuns(tuples);
//    }else{
//
//    }
    _Sort(tuples);
    int runSize = _runs.size();
    _runs.emplace_back(_files_per_run, tuples, _id, runSize);
    // _max_runs_before_merging.emplace_back(_files_per_run, tuples, _id, _max_runs_before_merging.size());
    return true;
}

bool Level::Scan(const Range& userAskedRange, Range& searchRange, 
        std::vector<Tuple*>& ret, std::vector<bool>& checkbits) {
    bool finished = false;

    // newly added data are at the back of the vector, thus check them in a reversed order
    for (auto rit = _runs.rbegin(); rit != _runs.rend(); rit++) {
        finished = rit->Scan(userAskedRange, searchRange, ret, checkbits);
        if (finished)
            break;
    }

    return finished;
}
