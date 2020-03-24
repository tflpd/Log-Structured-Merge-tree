#include <iostream>
#include <vector>
#include "Tuple.h"
#include "Log.h"
#include "Database.hpp"



int main(int argc, char **argv)
{

    templatedb::DB db;

    Value v1 = Value({1, 2});
    
    db.put(2, v1);
    db.put(1, v1);

    Value v2 = Value({4, 5, 7, 8, 9});
    db.put(4, v2);

    Value v3 = Value({0});
    db.put(9, v3);

    db.show_buf();

    return 0;
}
