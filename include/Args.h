#ifndef LSM_TREE_ARGS_H
#define LSM_TREE_ARGS_H

#ifndef NDEBUG
	// values dedicated for debugging
	#define SST_SIZE     36 /// Size of an sst file in BYTES
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
#define DEFAULT_FP_INTERV 2 /// THIS HAS TO BE AT LEAST EQUAL TO THE ELEMENTS CAPACITY OF SST AND AN EXACT DIVIDER OF THAT VALUE
#define DEFAULT_BF_NUM_TUPLES 1024
#define DEFAULT_BF_BITS_PER_ELEMENT 10
#define LEVELS_SIZE_RATIO 2 /// So for example level one runs will be equal to size ratio * level0_runs or equally level one tuples will be equal to size ratio * level0_tuples

#ifdef __linux__
#include <bits/stdc++.h>
#endif
#define TERMINATION      INT_MIN
#define DEL_PLACEHOLDER  INT_MAX

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

inline int getDefaultFPInterval() {
    return DEFAULT_FP_INTERV;
}

inline int getDefaultBFNumTuples() {
    return DEFAULT_BF_NUM_TUPLES;
}

inline int getDefaultBFBitsPerElement() {
    return DEFAULT_BF_BITS_PER_ELEMENT;
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

// @vector<bool> checkbits:
// this vector can be regarded as a list of buckets to indicate
// if any elements in [start, end] has been found previously
// ---------------------------------------------------------
// This function is used to compute the actual range [X, Y]
// that is needed to scan in current layer with prior knowledge that some buckets
// in checkbits has filled out
void FindStartEndPoint(int start, int end, 
	std::vector<bool>& checkbits, int& newStart, int& newEnd) {
	// found the first "hole"(first item with false) in checkbits
    // scan from then on
    int startPoint = start, endPoint = end;
    for (auto it = checkbits.begin(); it != checkbits.end(); it++) {
    	if (*it) startPoint++;
    }

    for (auto rit = checkbits.rbegin(); rit != checkbits.rend(); rit++) {
    	if (*rit) endPoint--;
    }

    newStart = startPoint;
    newEnd = endPoint;
}

#endif