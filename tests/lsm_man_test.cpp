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
    Value v2 = Value({4, 5});
    Value v3 = Value({333, 3});
    
    db.put(2345, v2);
    db.put(196, v3);
    db.put(3505, v2);
    db.put(4350, v3);
    db.put(4303, v2);
    db.put(2073, v3);
    db.put(4382, v2);
    db.put(3568, v3);
    db.put(2816, v2);
    db.put(2659, v3);

    std::vector<Tuple*> ret;
    db.scan(4313, 4402, ret);
    // for (auto ptr : ret) {
    //     auto key = ptr->GetKey();
    //     auto val = ptr->GetValue();
    //     std::cout << key << std::endl;
    //     for (auto v : val.items) {
    //         std::cout << v << " ";
    //     }
    //     std::cout << std::endl;
    // }
    for (int i = 0; i < ret.size(); i++) {
        int key = ret[i]->GetKey();
        cout << key << endl;
    }


    // auto ptuple = db.get(10);
    // auto val = ptuple->GetValue();
    // for (auto v : val.items) {
    //     std::cout << v << " ";
    // }
    // std::cout << std::endl;


    // db.del(10);
    // ptuple = db.get(10);
    // if (ptuple == nullptr) { std::cout << "del succeed!" << std::endl;}


    return 0;
}
