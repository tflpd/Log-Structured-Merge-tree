#include <iostream>
#include <vector>

#include "gtest/gtest.h"
#include "Database.hpp"
#include "Tuple.h"
#include "Log.h"
#include <unordered_map>
#include <vector>
#include <stdlib.h>
#include <time.h>

#ifdef __linux__
#include <bits/stdc++.h>
#endif

using namespace std;

#define INIT_RECORD_CNT 100000
#define MAX_KEY 50
#define MIN_KEY 1
#define MAX_VAL 10000
#define MIN_VAL 1
#define GET_REQUESTS 50000
#define DEL_REQUESTS 500
#define SCAN_REQUESTS 50000
#define MAX_SCAN_RANGE 30

class DBTest : public ::testing::Test
{
protected:
    templatedb::DB db;
    unordered_map<int, pair<int,int>> index;

    // get a rand key that exists in index map
    int GetRandKey() const {
        if (index.size() == 0) return INT_MIN;

        auto it = index.begin();
        int forwardDis = rand() % index.size();
        advance(it, forwardDis);

        return it->first;
    }

    void SetUp() override 
    {
        srand(time(0));

        // populate db w. some amount of data
        for (int cnt = 0; cnt < INIT_RECORD_CNT; cnt++) {
            int key = MIN_KEY + rand() % MAX_KEY;
            int val1 = MIN_VAL + rand() % MAX_VAL, val2 = MIN_VAL + rand() % MAX_VAL;
            Value val({val1, val2});

            index[key] = make_pair(val1, val2);
            db.put(key, val);
        }
    }
};


TEST_F(DBTest, GetFunctionality)
{
    for (int cnt = 0; cnt < GET_REQUESTS; cnt++) {
        int key = DBTest::GetRandKey();
        if (key == INT_MIN) {
            // cout << "GetFunctionality - DBTest index map is empty \n";
            break;
        }

        auto p_ret = DBTest::db.get(key);

        ASSERT_NE(p_ret, nullptr);

        auto val = p_ret->GetValue();
        auto pair = DBTest::index[key];
        Value expected({pair.first, pair.second});
        if (val == expected) {
            // cout << "success!" << endl;
        } else {
            // cout << "query key is: " << to_string(key) << endl;
            int expfirst = pair.first, expsec = pair.second;
            // cout << expfirst << ", " << expsec << endl;

            int retfirst = val.items[0], retsecond = val.items[1];
            // cout << retfirst << ", " << retsecond << endl;
        }

        EXPECT_EQ(val, expected);
        delete p_ret;
    }
}


TEST_F(DBTest, DeleteFunctionality)
{
    for (int cnt = 0; cnt < DEL_REQUESTS; cnt++) {
        int key = DBTest::GetRandKey();
        if (key == INT_MIN) {
            // cout << "DeleteFunctionality - DBTest index map is empty \n";
            break;
        }
        DBTest::db.del(key);
        DBTest::index.erase(key);

        auto p_ret = DBTest::db.get(key);
        EXPECT_EQ(p_ret, nullptr);
    }
}


TEST_F(DBTest, ScanFunctionality)
{
    for (int cnt = 0; cnt < SCAN_REQUESTS; cnt++) {
        // cout << endl;
        // cout << endl;
        int start = GetRandKey(), end = start + rand() % MAX_SCAN_RANGE;

        vector<Tuple*> ret;
        DBTest::db.scan(start, end, ret);

        // a dumb searching in unordered_map
        vector<Tuple> expected;
        for (int key = start; key <= end; key++) {
            if (DBTest::index.find(key) != DBTest::index.end()) {
                Value val({DBTest::index[key].first, DBTest::index[key].second});
                Tuple tu(key, val);
                // expected.emplace_back(move(tu));
                expected.push_back(tu);
            }
        }    

        ASSERT_EQ(expected.size(), ret.size()); 

        for (int i = 0; i < expected.size(); i++) {
            int exkey = expected[i].GetKey();
            int retkey = ret[i]->GetKey();

            EXPECT_EQ(exkey, retkey); // manually set failed

            auto expectedVal = expected[i].GetValue();
            auto retVal = ret[i]->GetValue();

            // if (!(retVal == expectedVal)) {
            //     string actual = "The DB actual scan result is: ";
            //     for (auto ptr : ret) {
            //         actual = actual + to_string(ptr->GetKey()) + ", "; 
            //     }
            //     cout << actual << endl;
            //     for (auto ptr : ret) {
            //         auto val = ptr->GetValue();
            //         cout << val.items[0] << ", " << val.items[1] << endl;
            //     }

            //     string expec = "The expected scan result is: ";
            //     for (auto& tu : expected) {
            //         expec = expec + to_string(tu.GetKey()) + ", ";
            //     }
            //     cout << expec << endl;
            //     for (auto& tu : expected) {
            //         auto val = tu.GetValue();
            //         cout << val.items[0] << ", " << val.items[1] << endl;
            //     }
            // }

            ASSERT_EQ(retVal, expectedVal);

            // for (auto p_tuple : ret) delete p_tuple;
        }
    }
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}