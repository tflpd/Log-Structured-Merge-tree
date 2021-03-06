#include "Level.h"
#include <vector>
#include "Args.h"
#define MEMORY_CAP 10800


Level::Level(uint level_id):
        _id(level_id) {
    _files_per_run = getBufferBytesSize() * getMaxRunsBeforeMerge() * pow(getLevelSizeRatio(), level_id)  / getSSTSize();
    if (_files_per_run == 0)
        _files_per_run++;
    _curr_tuples_n = 0;
}

Level::~Level() {
    for (auto pRuns : _runs)
        delete pRuns;
}

bool Level::ReadyMerge() const {
    return getMaxRunsBeforeMerge() == _runs.size() + 1;
}

/* assuming that the data to be pushed down is able to fit into memory */
// Takes the vector tuples and adds to it all the tuples of the runs of the below level to this vector
bool Level::_AddMergeRuns(vector<Tuple*>& tuples) {

    // For each one of the runs of this level starting from the last/latest
    for (int i = _runs.size() - 1; i >= 0; --i) {
        // Get its tuples
        auto runs_tuples = _runs.at(i)->GetAllTuples();
        // And add them to the aggregate vector that will hold all the tuples of the new level
        tuples.insert(tuples.end(), runs_tuples.begin(), runs_tuples.end());
    }
    // Delete the runs that were consumed to create the new level
    _Clear();

    return true;
}

bool Level::_Clear() {
    for (auto& pRun : _runs) {
        pRun->DeleteFMD(); 
        delete pRun;    
    }

    std::vector<Run*> tmp;
    _runs.swap(tmp);       
    return true;
}

// Merges two subarrays of tuples[].
// First subarray is tuples[left..mid]
// Second subarray is tuples[mid+1..right]
void merge(std::vector<Tuple*>& tuples, unsigned long left, unsigned long mid, unsigned long right)
{
//    cout << "Before:" << endl;
//    for(auto tup:tuples){
//        cout << tup->GetKey() << " ";
//    }
//    cout << endl;
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
        L.push_back((tuples)[left + i]);
    for (j = 0; j < n2; j++)
        R.push_back((tuples)[mid + 1 + j]);

    /* Merge the temp arrays back into tuples[left..right]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = left; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        //cout << L[i]->GetKey() << " <= " << R[j]->GetKey() << endl;
        if (L[i]->GetKey() <= R[j]->GetKey())
        {
            (tuples)[k] = L[i];
            i++;
            k++;
        }
        else
        {
            (tuples)[k] = R[j];
            j++;
            k++;
        }
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
//    cout << "After:" << endl;
//    for(auto tup:tuples){
//        cout << tup->GetKey() << " ";
//    }
//    cout << endl;
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
    vector<Tuple*> newTuples;
    int j = 0;
    newTuples.push_back(tuples.at(0));
    // Remove any duplicates keeping always the left most element as it is the most new
    for (int i = 1; i < tuples.size(); i++){
        if (newTuples.at(j)->GetKey() != tuples.at(i)->GetKey()){
            newTuples.push_back(tuples.at(i));
            j++;
        }
    }
    tuples = newTuples;
}


bool Level::AddNewRun(vector<Tuple*>& tuples) {
    _curr_tuples_n += tuples.size();
    _Sort(tuples);
    int runSize = _runs.size();
    auto ptr = new Run(_files_per_run, tuples, _id, runSize);
    _runs.push_back(ptr);
    return true;
}

bool Level::Scan(const Range& userAskedRange, Range& searchRange, 
        std::vector<Tuple*>& ret, std::vector<bool>& checkbits) {
    bool finished = false;
    // newly added data are at the back of the vector, thus check them in a reversed order
    for (auto rit = _runs.rbegin(); rit != _runs.rend(); rit++) {
        finished = (*rit)->Scan(userAskedRange, searchRange, ret, checkbits);
        if (finished)
            break;
    }

    return finished;
}
