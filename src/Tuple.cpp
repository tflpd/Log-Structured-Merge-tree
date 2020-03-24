//
// Created by Athanasios Filippidis on 2/25/20.
//

#include "Tuple.h"

#include <utility>

using namespace std;

Tuple::Tuple(std::string key, Value val):
    _key(std::move(key)), _value(std::move(val)){
}

Tuple::~Tuple() {}

void Tuple::PrintTuple() {
    //cout << "Key: " << _key << " Value: " << _value << endl;
}