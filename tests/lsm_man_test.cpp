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

    db.put(10, v2);
    db.put(5, v3);
    db.put(3, v3);
    db.put(6, v3);

    db.put(7, v3);

    db.put(8, v3);

    std::vector<Tuple*> ret;
    db.scan(2, 7, ret);
    for (auto ptr : ret) {
        auto key = ptr->GetKey();
        auto val = ptr->GetValue();
        std::cout << key << std::endl;
        for (auto v : val.items) {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }


    auto ptuple = db.get(10);
    auto val = ptuple->GetValue();
    for (auto v : val.items) {
        std::cout << v << " ";
    }
    std::cout << std::endl;


    db.del(10);
    ptuple = db.get(10);
    if (ptuple == nullptr) { std::cout << "del succeed!" << std::endl;}


    return 0;
}
