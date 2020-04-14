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

bool Buffer::Scan(const Range& searchRange, 
        std::vector<Tuple*>& ret, std::vector<bool>& checkbits) {
	int iStart = searchRange._begin, iEnd = searchRange._end;
	auto retIt = ret.begin();
	auto it = checkbits.begin();
	bool finished = true;

	// for current version, in-memory buffer is just an array and even not ordered
	// no special data structure used
	// we may move to skip list or any data structures that have a Log(n) search & insertion time
	// in the future
	for (auto pTuple : tuples) {
		int iKey = std::stoi(pTuple->GetKey());
		if (iKey >= iStart && iKey <= iEnd) {
			int diff = iKey - iStart;
			// deep copy it to return
			// since you never know when the buffer would release the data
			auto pData = new Tuple(*pTuple);
			*(retIt+diff) = pData;
			*(it + diff) = true;			
		} else {
			// even one key is lacked, we have to go deep into level to look for it
			finished &= false;
		}
	}

	return finished;
}

