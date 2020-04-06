#ifndef LSM_TREE_ARGS_H
#define LSM_TREE_ARGS_H

#ifndef NDEBUG
	// values dedicated for debugging
	#define SSTArgs     100
	#define CARRY       1
	#define MAXRUNS     2
	#define A 			1
#else  
	#define SSTArgs     64
	#define CARRY       1024
	#define MAXRUNS     3
	#define A 			1
#endif 

#define SIZEOFINT          4
#define PAGESIZE        4096
#define WRITEBUFCAP     4096
#define MAXTUPLEKEYCNTS    1
#define MAXTUPLEVALCNTS   10

#ifdef __linux__
#include <bits/stdc++.h>
#endif
#define TERMINATION      INT_MIN
#define DEL_PLACEHOLDER  INT_MAX

/* size of bytes */
inline int getSSTSize() {
	return SSTArgs * CARRY * CARRY; // from MB to B
}

inline int getTupleSize() {
	return (MAXTUPLEKEYCNTS + MAXTUPLEVALCNTS) * SIZEOFINT;
}

inline int getA() {
	return A;
}

inline int getMaxRuns() {
	return MAXRUNS;
}

#endif