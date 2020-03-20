#ifndef _TEMPLATEDB_OPERATION_H_
#define _TEMPLATEDB_OPERATION_H_

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace templatedb
{

typedef enum _operation_code
{
    GET = 0,
    SCAN = 1,
    DELETE = 2,
    PUT = 3,
    NO_OP = 100,
} op_code;


class Operation
{
public:
    op_code type;
    int key;
    std::vector<int> args;

    Operation() {type = NO_OP;};
    Operation(std::string op_string, int _key, std::vector<int> & _args);
    Operation(op_code _type, int _key, std::vector<int> & _args);

    static std::vector<Operation> ops_from_file(std::string file_name);
};

}   // namespace templatedb

#endif /* _TEMPLATEDB_OPERATION_H_ */