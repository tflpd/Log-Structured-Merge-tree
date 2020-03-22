#include <string>
#include <iostream>
#include <vector>

#include "gtest/gtest.h"
#include "Database.hpp"

TEST(PersistenceTest, BasicOpenClose)
{
    std::string fname = "test_db.db"; 
    templatedb::DB db;
    templatedb::Value v1({1, 2});
    db.open(fname);
    db.put(5, v1);
    db.close();

    db.open(fname);
    templatedb::Value v2 = db.get(5);
    db.close();
    remove(fname.c_str());
    ASSERT_EQ(v1, v2);
}

TEST(PersistenceTest, DeleteOpenClose)
{
    std::string fname = "test_db.db"; 
    templatedb::DB db;
    templatedb::Value v1({1, 2});
    templatedb::Value v2({12, 21});
    db.open(fname);
    db.put(5, v1);
    db.put(1, v2);
    db.del(5);
    db.close();

    db.open(fname);
    templatedb::Value v3 = db.get(5);
    templatedb::Value v4 = db.get(1);
    db.close();
    remove(fname.c_str());
    ASSERT_EQ(v3, templatedb::Value(false));
    ASSERT_EQ(v2, v4);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}