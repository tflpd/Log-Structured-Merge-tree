#include "Buffer.h"
#include "Log.h"
#include "Tuple.h"

Buffer::Buffer(int size): _max_cap(size) {
	// may change vector to dynamic allocated array in the future
	// DEBUG_LOG(std::string("creating a buffer with max size of #" + std::to_string(_max_cap)));
}

Buffer::~Buffer() {

}

bool Buffer::Append(std::string key, Value val) {
	// concurrent access may lead to tuple oversized
	// however, I think no special handling is needed 
	// as it doesn't hurt data integrity
	if (tuples.size() >= _max_cap) return false;
	
	auto* t = new Tuple(key, val);

	tuples.push_back(t);
	return true;
}

bool Buffer::IsFull() const {
	if (tuples.size() == _max_cap) return true;
	return false;
}

bool Buffer::Clear() {
	std::vector<Tuple*> tmp;
	tuples.swap(tmp);
	return true;
}

void Buffer::print() const {
	std::string output;
	for (auto& tuple : tuples) {
		output += tuple->ToString();
		output += " ";
	}
	KEY_LOG(output);
}

std::vector<Tuple*> Buffer::GetTuples() {
    return tuples;
}

bool Buffer::Scan(const Range& userAskedRange, Range& searchRange, 
        std::vector<Tuple*>& ret, std::vector<bool>& checkbits) {
	int iStart = userAskedRange._begin, iEnd = userAskedRange._end;
	int findCnt = 0;
	bool finished = true;

	// for current version, in-memory buffer is just an array and even not ordered
	// no special data structure used
	// we may move to skip list or any data structures that have a Log(n) search & insertion time
	// in the future
	for (auto rit = tuples.rbegin(); rit != tuples.rend(); rit++) {
		int iKey = std::stoi((*rit)->GetKey());
		int diff = iKey - iStart; 
		if (iKey >= iStart && iKey <= iEnd 
				&& !checkbits[diff]) {
			auto pData = new Tuple(**rit);
			ret[diff] = pData;
			checkbits[diff] = true;
			findCnt++;
		}
	}
	// not found all of them, narrow down the search range
	if (findCnt != checkbits.size()) {
		finished = false;
		ShrinkSearchRange(userAskedRange, searchRange, checkbits);
	}

	return finished;
}

