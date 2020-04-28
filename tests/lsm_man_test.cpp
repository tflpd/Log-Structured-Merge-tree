#include <iostream>
#include <vector>
#include "Tuple.h"
#include "Log.h"
#include "Database.hpp"
#include <sys/stat.h>
using namespace std;

inline bool exist(const char* filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

int main(int argc, char **argv)
{

    templatedb::DB db;
    
    Value v1 = Value({3059, 7667});
    db.put(1931, v1);
    Value v2 = Value({6457,2297});
    db.put(3718, v2);
    Value v3 = Value({833, 2337});
    db.put(768, v3);
    Value v4 = Value({2031, 9769});
    db.put(3252, v4);

    Value v5 = Value({5731, 9136});
    db.put(2770, v5);

    Value v6 = Value({3141, 3494});
    db.put(3976, v6);
    Value v7 = Value({8789, 1720});
    db.put(1369, v7);
    Value v8 = Value({4252, 8131});
    db.put(2031, v8);

    Value v9 = Value({2935, 1599});
    db.put(3135, v9);
    Value v10 = Value({3256, 1152});
    db.put(3235, v10);

    vector<pair<int, int>> scan_req = {{2031, 2118}, {1931, 1956}, {3252, 3273}, {1931, 1957}, {3235, 3299}};
    for (int i = 0; i < scan_req.size(); i++) {
        std::vector<Tuple*> ret;
        int start = scan_req[i].first, end = scan_req[i].second;
        db.scan(start, end, ret);
        cout << "scan range is:[" << start << ", " << end << "]" << endl;
        for (int i = 0; i < ret.size(); i++) {
            int key = ret[i]->GetKey();
            cout << key << endl;
            cout << ret[i]->_value.items[0] << ", " << ret[i]->_value.items[1] << endl; 
            cout << ret[i] << endl;
        }
        cout << endl;

//        cout << "EDW1" << endl;
//        delete ret[i];
//        cout << "EDW" << endl;
    }

    return 0;
}
