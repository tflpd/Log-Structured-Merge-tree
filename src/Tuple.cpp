//
// Created by Athanasios Filippidis on 2/25/20.
//

#include "Tuple.h"
#include "Args.h"
#include <stdlib.h>
#include <string.h>
#include <utility>

using namespace std;

Tuple::Tuple(std::string key, Value val):
    _key(std::move(key)), _value(std::move(val)){
}

// empty constructor for reading in persistent data on disk
Tuple::Tuple() {
    _value = Value();
}

Tuple::~Tuple() {}

void Tuple::PrintTuple() {
    //cout << "Key: " << _key << " Value: " << _value << endl;
}

std::string Tuple::ToString() const{
    string ret;
    ret += _key;
    ret += "|";

    ret += "{";
    for (auto& item: _value.items) {
        ret += std::to_string(item);
        ret += ",";
    }
	if (ret.back() != '{') ret.pop_back();
	ret += "}";

	return ret;
}

// @rbuf: starting point of this tuple in the whole array
void Tuple::AppendBin2Vec(char* wbuf) const {
    int cap = getTupleBytesSize();
    char* tmp = new char[cap];
    memset(tmp, 0, cap); 

    // convert _key from int to char(byte) and fill it in char array
    int k = std::stoi(_key);
    memcpy(tmp, &k, SIZEOFINT);

    int num2copy = (_value.items.size() <= MAX_TUPLE_VALUES_CNTS) ? _value.items.size() : MAX_TUPLE_VALUES_CNTS;
    memcpy(tmp + SIZEOFINT, &_value.items[0], num2copy*SIZEOFINT);

    // if there's still empty space left, add up a 'marker'
    // to indicate termination
    if (num2copy + 1 < MAX_TUPLE_VALUES_CNTS) {
        int terminateMarker = TERMINATION;
        memcpy(tmp + SIZEOFINT + num2copy*SIZEOFINT, &terminateMarker, SIZEOFINT);
    }

    // copy tmp back to wbuf
    memcpy(wbuf, tmp, cap);
    delete[] tmp;
}

// @rbuf: starting point of this tuple in the whole array
void Tuple::Read2Tuple(char* rbuf) {
    auto size = getTupleBytesSize();

    // TODO: may copy necessary chunk of data and start working in a newly spawned thread
    // to optimize performance
    // not sure the copy process is more costly than working paralel
    // std::vector<char> tmpbuf(rbuf, rbuf+pos);

    // first MAX_TUPLE_KEYS_CNTS * SIZEOFINT bytes refer to key
    int size_of_key = MAX_TUPLE_KEYS_CNTS * SIZEOFINT;
    int key;
    std::memcpy(&key, rbuf, SIZEOFINT);
    _key = to_string(key);

    // iterate through the following bytes util meets a val of TERMINATION
    // dumb approach
    // TODO: any better alternative?
    char* pos = rbuf + size_of_key;
    for (int cnt = 0; cnt < MAX_TUPLE_VALUES_CNTS; cnt++) {
        int val;
        std::memcpy(&val, pos, SIZEOFINT);

        if (val == TERMINATION) break;
        _value.items.push_back(val);

        pos += SIZEOFINT;
    }
}

