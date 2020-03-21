//
// Created by Athanasios Filippidis on 2/25/20.
//

#ifndef LSM_TREE_TUPLE_H
#define LSM_TREE_TUPLE_H

#include <iostream>
#include "Database.hpp"

class Tuple{
private:
    std::string _key;
    templatedb::Value _value;
public:
    Tuple(std::string key, templatedb::Value Value);
    ~Tuple();
//    inline void SetKey(const std::string key){
//        this->_key = key;
//    }
    inline void SetValue(const templatedb::Value Value){
        this->_value = Value;
    }
    inline std::string GetKey(){
        return _key;
    }
    inline templatedb::Value GetValue(){
        return _value;
    }
    void PrintTuple();
};


#endif //LSM_TREE_TUPLE_H
