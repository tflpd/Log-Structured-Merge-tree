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
    
    Value v1 = Value({1, 1});
    db.put(28, v1);
    Value v2 = Value({1,1});
    db.put(21, v2);
    Value v3 = Value({2, 2});
    db.put(28, v3);
    Value v4 = Value({1, 1});
    db.put(13, v4);

    Value v5 = Value({1, 1});
    db.put(49, v5);
    Value v6 = Value({1, 1});
    db.put(18, v6);
    Value v7 = Value({1, 1});
    db.put(39, v7);
    Value v8 = Value({1, 1});
    db.put(43, v8);

    Value v9 = Value({1, 1});
    db.put(4, v9);
    Value v10 = Value({1, 1});
    db.put(49, v10);
    Value v11 = Value({2, 2});
    db.put(21, v11);
    Value v12 = Value({1, 1});
    db.put(41, v12);

    Value v13 = Value({1, 1});
    db.put(48, v13);
    Value v14 = Value({1, 1});
    db.put(12, v14);
    Value v15 = Value({1, 1});
    db.put(8, v15);
    Value v16 = Value({1, 1});
    db.put(20, v16);

    Value v17 = Value({1, 1});
    db.put(26, v17);
    Value v18 = Value({2, 2});
    db.put(20, v18);
    Value v19 = Value({3, 3});
    db.put(20, v19);
    Value v20 = Value({1, 1});
    db.put(18, v20);


   vector<pair<int, int>> scan_req = {{41, 46}};
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
   }

    return 0;
}
