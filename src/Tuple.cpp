//
// Created by Athanasios Filippidis on 2/25/20.
//

#include "include/Tuple.h"

using namespace std;

Tuple::Tuple(int key, int value) {
    this->key = key;
    this->value = value;
}

Tuple::~Tuple() {}

void Tuple::PrintTuple() {
    cout << "Key: " << key << " Value: " << value << endl;
}