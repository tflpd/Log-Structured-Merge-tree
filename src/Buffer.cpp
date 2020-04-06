#include "Buffer.h"
#include "Log.h"
#include "Tuple.h"

Buffer::Buffer(int size): _max_cap(size) {
	// may change vector to dynamic allocated array in the future
	DEBUG_LOG(std::string("creating a buffer with max size of #" + std::to_string(_max_cap)));
}

Buffer::~Buffer() {

}

bool Buffer::Append(std::string key, Value val) {
	// concurrent access may lead to tuple oversized
	// however, I think no special handling is needed 
	// as it doesn't hurt data integrity
	if (tuples.size() >= _max_cap) return false;
	
	Tuple* t = new Tuple(key, val);

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



