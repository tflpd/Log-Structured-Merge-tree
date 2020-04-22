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

using namespace std;

#define INIT_RECORD_CNT 1000
#define MAX_KEY 5000
#define MAX_VAL 10000
#define GET_REQUESTS 50
#define DEL_REQUESTS 50
#define SCAN_REQUESTS 50
#define MAX_SCAN_RANGE 100

class DBTest : public ::testing::Test
{
    DB db();
    unordered_map<int, pair<int,int>> index;

    // get a rand key that exists in index map
    void GetRandKey() const {

    }

    void SetUp() override 
    {
        srand(time(0));

        // populate db w. some amount of data
        for (int cnt = 0; cnt < INIT_RECORD_CNT; cnt++) {
            int key = rand() % MAX_KEY;
            int val1 = rand() % MAX_VAL, val2 = rand() % MAX_VAL;
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
        auto p_ret = DBTest::db.get(key);

        ASSERT_NE(p_ret, nullptr);

        auto val = p_ret->GetValue();
        auto pair = DBTest::index[key];
        Value expected({pair.first, pair.second});

        EXPECT_EQ(val, expected);
    }
}


TEST_F(DBTest, DeleteFunctionality)
{
    for (int cnt = 0; cnt < DEL_REQUESTS; cnt++) {
        int key = DBTest::GetRandKey();
        DBTest::db.del(key);
        DBTest::index.erase(key);

        auto p_ret = DBTest::db.get(key);
        EXPECT_EQ(p_ret, nullptr);
    }
}


TEST_F(DBTest, ScanFunctionality)
{
    for (int cnt = 0; cnt < SCAN_REQUESTS; cnt++) {
        int start = rand() % MAX_KEY, end = start + rand() % MAX_SCAN_RANGE;

        vector<Tuple*> ret;
        DBTest::db.scan(start, end, ret);

            

    }
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}