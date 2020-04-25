//
// Created by Athanasios Filippidis on 2/25/20.
//

#ifndef LSM_TREE_TUPLE_H
#define LSM_TREE_TUPLE_H

#include <iostream>
#include <vector>
#include "Args.h"
// #include "Database.hpp"

class Value
{
public:
    std::vector<int> items;
    bool visible = true;

    Value() {}
    // Value(bool _visible) {visible = _visible;}
    Value(std::vector<int> _items) { items = _items;}
    Value(const Value& other) {
        visible = other.visible;
        items.insert(items.end(), other.items.begin(), other.items.end());
    }
    
    bool operator ==(Value const & other) const
    {
        if (other.items.size() != items.size()) return false;
        for (int i = 0; i < items.size(); i++) 
            if (items[i] != other.items[i])
                return false;

        return true; 
    }
};


class Tuple{
private:
    std::string _key;
    Value _value;
public:
    Tuple(std::string key, Value val);
    ~Tuple();

    Tuple(const Tuple& other);
    Tuple();
//    inline void SetKey(const std::string key){
//        this->_key = key;
//    }
    inline void SetValue(Value val){
        this->_value = val;
    }
    inline std::string GetKey() const{
        return _key;
    }
    inline Value GetValue() const{
        return _value;
    }
    std::string ToString() const;

    inline bool IsDelMarker() const {
        return *_value.items.begin() == TERMINATION;
    }
    
    void AppendBin2Vec(char* wbuf) const;
    void Read2Tuple(char* rbuf);

    void PrintTuple();
};

// #include "Tuple.cpp"
#endif //LSM_TREE_TUPLE_H
