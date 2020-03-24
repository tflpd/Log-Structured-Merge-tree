#include <iostream>
#include <vector>
#include "Tuple.h"
#include "Database.hpp"



int main(int argc, char **argv)
{
    templatedb::DB db;
    Value v1 = Value({1, 2});
    db.put(2, v1);
    return 0;
}
