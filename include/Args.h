#ifndef LSM_TREE_ARGS_H
#define LSM_TREE_ARGS_H


#ifndef NDEBUG
	// values dedicated for debugging
	#define SST_SIZE     48 /// Size of an sst file in BYTES
	#define CARRY       50 /// The real sst size will be the above times CARRY^2
	#define MAX_RUNS_BEFORE_MERGE     3 /// Every level will merge after the creation of the MAX_RUNS_BEFORE_MERGE run in it
	#define A 			1
#else  
	#define SST_SIZE     48	
	#define CARRY       50
	#define MAX_RUNS_BEFORE_MERGE     10
	#define A 			1
#endif 

#define SIZEOFINT          4
#define PAGESIZE        4096
#define WRITEBUFCAP     4096
#define MAX_TUPLE_KEYS_CNTS    1 /// The number of keys per tuple
#define MAX_TUPLE_VALUES_CNTS   2 /// The number of values per tuple
#define FP_INTERV 50 /// DETERMINES HOW MANY KEYS WILL EXIST BETWEEN TWO FENCE POINTERS. E.G. IF WE HAVE THE 1 4 5 6 KEYS IN A RUN AND AN INTERVAL = 1 THEN THE FPS WILL BE 1, 5, 6
#define BF_NUM_TUPLES 4*CARRY*CARRY /// FOR NOW this should be a one to one relation to the sst size - so one bf per file
#define BF_BITS_PER_ELEMENT 8 /// This determines the precision of the Bloom filters, more bits per element leads to higher precision, 32 bits would lead to each integer be represented fully
#define LEVELS_SIZE_RATIO 2 /// So for example, level1's runs will be equal to size ratio * level0_runs or equally level1's tuples will be equal to size ratio * level0_tuples

#ifdef __linux__
#include <bits/stdc++.h>
#endif
#define TERMINATION      INT_MIN
#define DEL_PLACEHOLDER  INT_MAX

#include <vector>
#include <iostream>

/* size of bytes */
inline int getSSTSize() {
	return SST_SIZE * CARRY * CARRY; // from MB to B
}

inline int getTupleBytesSize() {
	return (MAX_TUPLE_KEYS_CNTS + MAX_TUPLE_VALUES_CNTS) * SIZEOFINT;
}

inline int getA() {
	return A;
}

inline int getMaxRunsBeforeMerge() {
	return MAX_RUNS_BEFORE_MERGE;
}

inline int getFPInterval() {
    return FP_INTERV;
}

inline int getBFNumTuples() {
    return BF_NUM_TUPLES;
}

inline int getBFBitsPerElement() {
    return BF_BITS_PER_ELEMENT;
}

inline int getBufferBytesSize() {
    return A * SST_SIZE;
}

inline int getBufferElementsSize() {
    return getBufferBytesSize() / getTupleBytesSize();
}

inline int getLevelSizeRatio() {
    return LEVELS_SIZE_RATIO;
}

inline bool AllTrue(std::vector<bool>& checkbits) {
	bool found = true;
	for (auto exist : checkbits)
		found &= exist;
	return found;
}

struct Range {
	int _begin;
	int _end;
	Range() {}
	Range(int begin, int end) : _begin(begin), _end(end) {}
	Range(const Range& other) {
		_begin = other._begin;
		_end = other._end;
	}
};

#include "Log.h"

// @vector<bool> checkbits:
// this vector can be regarded as a list of buckets to indicate
// if any elements in [start, end] has been found previously
// @userAskedRange:
// 		search range asked by client, defined at Database.cpp
// @searchRange:
//	    current shrinked search range during processing progress, intending to 
//      avoid overheads 
// ---------------------------------------------------------
// This function is used to compute the actual range [X, Y]
// that is needed to scan in current layer with prior knowledge that some buckets
// in checkbits has filled out
static void ShrinkSearchRange(const Range& userAskedRange, Range& searchRange, 
	std::vector<bool>& checkbits) {
	// found the first "hole"(first item with false) in checkbits
    // scan from then on
    int startPoint = userAskedRange._begin, endPoint = userAskedRange._end;
    for (auto it = checkbits.begin(); it != checkbits.end(); it++) {
    	if (*it == false) break; 
    	startPoint++;
    }

    for (auto rit = checkbits.rbegin(); rit != checkbits.rend(); rit++) {
    	if (*rit == false) break; 
    	endPoint--;
    }

    searchRange._begin = startPoint;
    searchRange._end = endPoint;

    // std::string log = "newly shrinked search range is from [" + 
    	// std::to_string(startPoint) + " ," + std::to_string(endPoint) + "]";
    // DEBUG_LOG(log);

}

#endif