#ifndef LSM_TREE_LOG_H
#define LSM_TREE_LOG_H

#ifndef NDEBUG
#define DEBUG_LOG(x) \
	do { std::cerr << x << std::endl; } \
	while(0) 
#else
#define DEBUG_LOG(x) \
	do {} \
	while(0)
#endif

#define KEY_LOG(x) do { std::cerr << x << std::endl; } while(0)


#include "Args.h"

inline void PrintRange(const Range& range) {
	std::cout << "range [" << range._begin << ", " << range._end << "]" << std::endl;
}

#endif
