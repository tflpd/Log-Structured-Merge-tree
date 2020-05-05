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
    
    Value v1 = Value({534, 654});
    db.put(35, v1);
    Value v2 = Value({324,6574});
    db.put(23, v2);
    Value v3 = Value({65462, 5343});
    db.put(35, v3);
    Value v4 = Value({54313, 877});
    db.put(30, v4);

    Value v5 = Value({6, 87});
    db.put(43, v5);
    Value v6 = Value({55, 6356});
    db.put(40, v6);
    Value v7 = Value({54, 45742});
    db.put(43, v7);
    Value v8 = Value({65, 8131});
    db.put(1, v8);

    Value v9 = Value({65465, 6});
    db.put(33, v9);
    Value v10 = Value({7, 12});
    db.put(36, v10);
    Value v11 = Value({2935, 57});
    db.put(26, v11);
    Value v12 = Value({243, 4555});
    db.put(39, v12);

    Value v13 = Value({54, 54});
    db.put(20, v13);
    // Value v14 = Value({66, 66});
    // db.put(49, v14);
    // Value v15 = Value({416, 245});
    // db.put(21, v15);
    // Value v16 = Value({645, 76});
    // db.put(15, v16);

    // Value v17 = Value({8, 6});
    // db.put(21, v17);
    // Value v18 = Value({5464, 675});
    // db.put(21, v18);
    // Value v19 = Value({1, 2});
    // db.put(13, v19);
    // Value v20 = Value({3, 4});
    // db.put(38, v20);


   vector<pair<int, int>> scan_req = {{43, 55}};
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
