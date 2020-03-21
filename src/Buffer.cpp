
// #ifndef LSM_TREE_BUFFER_H
// #define LSM_TREE_BUFFER_H

// #include "Tuple.h"

// #define SIZE 10

// class Buffer{
// private:
//     Tuple *tuples;
// public:
//     Buffer(int size);
//     ~Buffer();

//     bool Clear();
//     bool Get(Buffer* other); // move data from other buffer to this buffer
// };

#include "Buffer.h"

Buffer::Buffer(int size) {

}

Buffer::~Buffer() {

}

bool Buffer::Append(int key, int val) {
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



