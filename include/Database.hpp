#ifndef _TEMPLATEDB_DB_H_
#define _TEMPLATEDB_DB_H_

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "Operation.hpp"
#include "LSM_T.h"
#include "Tuple.h"

namespace templatedb
{

typedef enum _status_code
{
    OPEN = 0,
    CLOSED = 1,
    ERROR_OPEN = 100,
} db_status;


class DB
{
public:
    db_status status;

    DB();
    ~DB();

    Tuple* get(int key);
    void put(int key, Value val);
    void scan(int min_key, int max_key, std::vector<Tuple*>& ret);
    void del(int key);

    void show_buf() const;
    // size_t size();

    // db_status open(std::string & fname);
    // bool close();

    // bool load_data_file(std::string & fname);

    // std::vector<Value> execute_op(Operation op);

private:
    std::fstream file;
    // std::unordered_map<int, Value> table;
    LSM_T* table = nullptr;
    size_t value_dimensions = 0;
    
    // bool write_to_file();
};

}   // namespace templatedb

#endif /* _TEMPLATEDB_DB_H_ */