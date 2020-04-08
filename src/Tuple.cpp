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

void Tuple::AppendBin2Vec(std::vector<char>& wbuf, int pos) const {
    int cap = getTupleSize();
    char* tmp = new char[cap];
    memset(tmp, 0, cap); 

    // convert _key from int to char(byte) and fill it in char array
    int k = std::stoi(_key);
    memcpy(tmp, &k, SIZEOFINT);
    char* valStartAddr = tmp + SIZEOFINT;

    // these 2 variables are numbers of items in vecotr
    int maxValCnts = (cap - SIZEOFINT) / SIZEOFINT;
    int num2copy = (_value.items.size() <= maxValCnts)? _value.items.size() : maxValCnts;

    memcpy(valStartAddr, &_value.items[0], num2copy*SIZEOFINT);

    // if there's still empty space left, add up a 'marker'
    // to indicate termination
    if (num2copy + 1 < maxValCnts) {
        int terminateMarker = TERMINATION;
        memcpy(valStartAddr+num2copy*SIZEOFINT, &terminateMarker, SIZEOFINT);
    }

    // copy tmp back to wbuf
    memcpy(&wbuf[0] + pos, tmp, cap);
    delete[] tmp;
}

// @rbuf: starting point of this tuple in the whole array
void Tuple::Read2Tuple(char* rbuf) {
    auto size = getTupleSize();

    // TODO: may copy necessary chunk of data and start working in a newly spawned thread
    // to optimize performance
    // not sure the copy process is more costly than working paralel
    // std::vector<char> tmpbuf(rbuf, rbuf+pos);

    // first MAXTUPLEKEYCNTS * SIZEOFINT bytes refer to key
    int size_of_key = MAXTUPLEKEYCNTS * SIZEOFINT;
    _key = std::string(rbuf, size_of_key);

    // iterate through the following bytes util meets a val of TERMINATION
    // dumb approach
    // TODO: any better alternative?
    char* pos = rbuf + size_of_key;
    for (int cnt = 0; cnt < MAXTUPLEVALCNTS; cnt++) {
        int val;
        std::memcpy(&val, pos, SIZEOFINT);

        if (val == TERMINATION) break;
        _value.items.push_back(val);

        pos += SIZEOFINT;
    }
}

