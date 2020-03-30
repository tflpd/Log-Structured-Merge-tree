#include <iostream>
#include <vector>
#include <Tuple.h>

#include "Database.hpp"

using namespace std;
//// Merges two subarrays of tuples[].
//// First subarray is tuples[left..mid]
//// Second subarray is tuples[mid+1..right]
//void merge(std::vector<Tuple*> *tuples, int left, int mid, int right)
//{
//    int i, j, k;
//    int n1 = mid - left + 1;
//    int n2 = right - mid;
//
//    /* create temp arrays */
//    //int L[n1], R[n2];
//    std::vector<Tuple*> L;
//    L.reserve(n1);
//    std::vector<Tuple*> R;
//    R.reserve(n2);
//
//    /* Copy data to temp arrays L[] and R[] */
//    for (i = 0; i < n1; i++)
//        L[i] = (*tuples)[left + i];
//        //L[i] = *tuples->at(left + i);
//        //L.push_back(tuples->at(left + i));
//    for (j = 0; j < n2; j++)
//        R[j] = (*tuples)[mid + 1 + j];
//        //R.push_back(tuples->at(mid + 1 + j));
//
//    /* Merge the temp arrays back into tuples[left..right]*/
//    i = 0; // Initial index of first subarray
//    j = 0; // Initial index of second subarray
//    k = left; // Initial index of merged subarray
//    cout << "WREE" << endl;
//    while (i < n1 && j < n2)
//    {
//        if (stoi(L[i]->GetKey()) <= stoi(R[j]->GetKey()))
//        {
//            (*tuples)[k] = L[i];
//            i++;
//        }
//        else
//        {
//            (*tuples)[k] = R[j];
//            j++;
//        }
//        k++;
//    }
//
//    /* Copy the remaining elements of L[], if there
//       are any */
//    while (i < n1)
//    {
//        (*tuples)[k] = L[i];
//        i++;
//        k++;
//    }
//
//    /* Copy the remaining elements of R[], if there
//       are any */
//    while (j < n2)
//    {
//        (*tuples)[k] = R[j];
//        j++;
//        k++;
//    }
//
//}
//
///* left is for left index and right is right index of the
//   sub-array of arr to be sorted */
//void mergeSort(std::vector<Tuple*> *arr, int left, int right)
//{
//    cout << "Ksekinaw me " << left << " " << right << endl;
//
//    if (left < right)
//    {
//        // Same as (left+right)/2, but avoids overflow for
//        // large left and right
//        int mid = left + (right - left) / 2;
//
//        // Sort first and second halves
//        mergeSort(arr, left, mid);
//        mergeSort(arr, mid + 1, right);
//
//        merge(arr, left, mid, right);
//    }
//}

int main(int argc, char **argv)
{
//    templatedb::DB db;
//    templatedb::Value v1 = templatedb::Value({1, 2});
//    db.put(2, v1);
// Merge sort functionality basic test below
//    auto *t1 = new Tuple("2", nullptr);
//    auto *t2 = new Tuple("0", nullptr);
//    auto *t3 = new Tuple("1821", nullptr);
//    std::vector<Tuple*> tuples;
//    tuples.push_back(t3);
//    tuples.push_back(t1);
//    tuples.push_back(t2);
//    mergeSort(&tuples, 0, tuples.size() - 1);
//    for (int i = 0; i < tuples.size(); ++i) {
//        std::cout << tuples.at(i)->GetKey() << std::endl;
//    }

    return 0;
}