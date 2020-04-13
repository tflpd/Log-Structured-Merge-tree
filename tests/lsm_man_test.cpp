#include <iostream>
#include <vector>
#include "Tuple.h"
#include "Log.h"
#include "Database.hpp"
#include <sys/stat.h>

inline bool exist(const char* filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

int main(int argc, char **argv)
{

    templatedb::DB db;
    Value v1 = Value({1, 2});
    db.put(2, v1);
    db.put(1, v1);

    Value v2 = Value({4, 5});
    db.put(4, v2);
    Value v3 = Value({333, 3});
    db.put(9, v3);

    db.put(10, v3);
    db.put(5, v3);
    db.put(3, v3);
    db.put(7, v3);

    db.put(8, v3);


    auto v = db.get(4);

    return 0;
}
