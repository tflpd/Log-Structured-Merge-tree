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
// TODO: Ensure that tuples do not have doublicates when passed here
bool Level::_AddMergeRuns(vector<Tuple*>& tuples) {

    // Add all tuples of this level in a vector
//    for (auto& run : _runs) {
//        auto arr = run.GetAllTuples();
//        tuples.insert(tuples.end(), arr.begin(), arr.end());
//    }
    // For each one of the runs of this level starting from the last/latest
    for (int i = _runs.size() - 1; i >= 0; --i) {
        // Get its tuples
        //std::cout << "EDW1 " <<  to_string(i) << endl;
        auto runs_tuples = _runs.at(i).GetAllTuples();
        //std::cout << "EDW2" << endl;
        // For every tuple in the vector that was dumped from the level above
        for(auto& tuple : tuples){
            //std::cout << "EDW3" << endl;
            // Try to get the position of this tuple in the run we are going through right now
            // using binary search since each run is already sorted
            auto it = std::lower_bound(
                    runs_tuples.begin(), runs_tuples.end(), tuple,
                    [](Tuple *t1, Tuple *t) { return t1->GetKey() < t->GetKey(); });
            // If we actually found the position of a tuple with the same key in the run we
            // are currently going through
            if ( it != runs_tuples.end() && (*it)->GetKey() == tuple->GetKey() ){
                // Then delete this tuple since we have already a "fresher" value of it
                runs_tuples.erase(it);
            }
        }
        // After we have deleted all the tuples that have "fresher" values
        // we can merge the rest to the "tuples" vector and continue the process
        // for the rest of the runs
        tuples.insert(tuples.end(), runs_tuples.begin(), runs_tuples.end());
    }

    _Clear();

    return true;
}

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
        if (L[i]->GetKey() <= R[j]->GetKey())
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
