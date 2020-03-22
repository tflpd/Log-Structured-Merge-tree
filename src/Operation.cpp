#include "Operation.hpp"

using namespace templatedb;

Operation::Operation(std::string op_string, int _key, std::vector<int> & _args)
{
    if (op_string == "PUT")
        type = PUT;
    else if (op_string == "GET")
        type = GET;
    else if (op_string == "SCAN")
        type = SCAN;
    else if (op_string == "DELETE")
        type = DELETE;
    else
        type = NO_OP;
    key = _key;
    args = _args;
}


Operation::Operation(op_code _type, int _key, std::vector<int> & _args)
{
    type = _type;
    key = _key;
    args = _args;
}


std::vector<Operation> Operation::ops_from_file(std::string file_name)
{
    std::ifstream fid(file_name);
    std::vector<Operation> ops;
    std::vector<int> args;
    if (fid.is_open())
    {
        std::string line, item, op_string, key;
        std::getline(fid, line); // First line is number of ops

        int num_ops = stoi(line);
        ops.reserve(num_ops);
        while (std::getline(fid, line))
        {
            args = std::vector<int>();

            std::stringstream linestream(line);
            std::getline(linestream, op_string, ','); // First line is an op_code
            std::getline(linestream, key, ','); // First argument is a key
            while(std::getline(linestream, item, ','))
            {
                args.push_back(stoi(item));
            }
            ops.push_back(Operation(op_string, stoi(key), args));
        }
    }
    else
    {
        printf("Unable to read %s\n", file_name.c_str());
    }

    return ops;
}