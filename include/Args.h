#ifndef LSM_TREE_ARGS_H
#define LSM_TREE_ARGS_H

#ifndef NDEBUG
	// values dedicated for debugging
	#define SST_SIZE     48 /// Size of an sst file in BYTES
	#define CARRY       1
	#define MAX_RUNS_BEFORE_MERGE     2
	#define A 			1
#else  
	#define SST_SIZE     64
	#define CARRY       1024
	#define MAX_RUNS_BEFORE_MERGE     3
	#define A 			1
#endif 

#define SIZEOFINT          4
#define PAGESIZE        4096
#define WRITEBUFCAP     4096
#define MAX_TUPLE_KEYS_CNTS    1
#define MAX_TUPLE_VALUES_CNTS   2
#define FP_INTERV 1 /// THIS HAS TO BE EQUAL OR LESS TO THE ELEMENTS CAPACITY OF SST AND (FP_INTERV + 1) MUST BE AN EXACT DIVIDER OF THAT VALUE (SO MOD SHOULD BE ZERO)
#define BF_NUM_TUPLES 4 /// FOR NOW this should be a one to one relation to the sst size - so one bf per file
#define BF_BITS_PER_ELEMENT 1 /// The relation between BF_NUM_TUPLES and BF_BITS_PER_ELEMENT should be ~ 10:1
#define LEVELS_SIZE_RATIO 2 /// So for example level one runs will be equal to size ratio * level0_runs or equally level one tuples will be equal to size ratio * level0_tuples

#ifdef __linux__
#include <bits/stdc++.h>
#endif
#define TERMINATION      INT_MIN
#define DEL_PLACEHOLDER  INT_MAX

#include <vector>

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
    for (auto it = checkbits.begin(); it != checkbits.end(); it++) 
    	if (*it) startPoint++;
 

    for (auto rit = checkbits.rbegin(); rit != checkbits.rend(); rit++) 
    	if (*rit) endPoint--;

    searchRange._begin = startPoint;
    searchRange._end = endPoint;

    std::string log = "newly shrinked search range is from [" + 
    	std::to_string(startPoint) + " ," + std::to_string(endPoint) + "]";
    // DEBUG_LOG(log);

}

#endif