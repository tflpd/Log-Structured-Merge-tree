//
// Created by Athanasios Filippidis on 2/25/20.
//

#ifndef LSM_TREE_TUPLE_H
#define LSM_TREE_TUPLE_H

#include <iostream>

class Tuple{
private:
    unsigned int key;
    unsigned int value;
public:
    Tuple(int key, int value);
    ~Tuple();
    inline void SetKey(const unsigned int key){
        this->key = key;
    }
    inline void SetValue(const unsigned int value){
        this->value = value;
    }
    inline int GetKey(){
        return key;
    }
    inline int GetValue(){
        return value;
    }
    void PrintTuple();
};


#endif //LSM_TREE_TUPLE_H
