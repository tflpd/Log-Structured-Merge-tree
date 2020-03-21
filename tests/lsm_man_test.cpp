#include <iostream>
#include <vector>

#include "Database.hpp"



int main(int argc, char **argv)
{
    templatedb::DB db;
    templatedb::Value v1 = templatedb::Value({1, 2});
    db.put(2, v1);
    return 0;
}