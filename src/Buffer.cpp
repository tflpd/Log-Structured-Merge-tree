// class Buffer{
// private:
//     std::vector<Tuple> tuples;
// public:
//     Buffer(int size);
//     ~Buffer();

//     bool Clear();
//     bool Get(Buffer* other); // move data from other buffer to this buffer

//     bool Append(std::string key, Value val);
//     bool IsFull() const;
// };

#include "Buffer.h"
#include "Log.h"

Buffer::Buffer(int size): _max_cap(size) {
	// may change vector to dynamic allocated array in the future
}

Buffer::~Buffer() {

}

bool Buffer::Append(std::string key, Value val) {
	// concurrent access may lead to tuple oversized
	// however, I think no special handling is needed 
	// as it doesn't hurt data integrity 
	if (tuples.size() >= _max_cap) return false;

	tuples.emplace_back(key, val);
	return true;
}

bool Buffer::IsFull() const {
	return true;
}

bool Buffer::Clear() {
	return true;
}

bool Buffer::Get(Buffer* other) {
	return true;
}

void Buffer::print() const {
	std::string output;
	for (auto& tuple : tuples) {
		output += tuple.ToString();
		output += " ";
	}
	KEY_LOG(output);
}



