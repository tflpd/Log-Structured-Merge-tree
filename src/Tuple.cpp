//
// Created by Athanasios Filippidis on 2/25/20.
//

#include "../include/Tuple.h"

#include <utility>

using namespace std;

Tuple::Tuple(std::string Key, templatedb::Value Value):
    _key(std::move(Key)), _value(std::move(Value)){
}

Tuple::~Tuple() {}

void Tuple::PrintTuple() {
    //cout << "Key: " << _key << " Value: " << _value << endl;
}