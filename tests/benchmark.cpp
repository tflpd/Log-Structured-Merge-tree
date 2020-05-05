#include <string>
#include <iostream>
#include <vector>

#ifdef __linux__
#include <bits/stdc++.h>
#endif

#include <chrono> 
#include <stdlib.h>
#include <time.h>
#include <numeric>
#include "Tuple.h"
#include "Database.hpp"

// #define TOTAL_OPERATION 5000
#define TOTAL_OPERATION 50000
#define READ_PERCENT  0.8
#define GET_PERCENT   0.5
#define SCAN_PERCENT  0.5

#define WRITE_PERCENT 0.2
#define PUT_PERCENT   0.5
#define DEL_PERCENT   0.5

#define OPERATION_CODE_GET    0
#define OPERATION_CODE_SCAN   1
#define OPERATION_CODE_DELETE 2
#define OPERATION_CODE_PUT    3

#define KEY_MAX 100
#define VALUE_MAX 10000
#define SCAN_RANGE_LIMIT 50

struct OperationBase {
    int OperationCode;
    templatedb::DB* pDB = nullptr;
    OperationBase(int code) { OperationCode = code; }
    void bindDB(templatedb::DB& db) {
        pDB = &db;
    }
    virtual void execute() = 0;
};

struct OperationGetDel : OperationBase {
    int Key;
    OperationGetDel(int code) : OperationBase(code) {
        Key = rand() % KEY_MAX;
    }

    void execute() {
        if (pDB == nullptr) return;

        if (OperationCode == OPERATION_CODE_GET) pDB->get(Key);
        else if (OperationCode == OPERATION_CODE_DELETE) pDB->del(Key);
    }
};

struct OperationPut : OperationBase {
    int Key;
    Value Val;
    OperationPut(int code) : OperationBase(code) {
        Key = rand() % KEY_MAX;
        Val = Value({rand() % VALUE_MAX, rand() % VALUE_MAX});
    }
    void execute() {
        if (pDB == nullptr) return;
        pDB->put(Key, Val);
    }
};

struct OperationScan : OperationBase {
    int startKey, endKey;
    OperationScan(int code) : OperationBase(code) {
        startKey = rand() % KEY_MAX;
        endKey = startKey + rand() % SCAN_RANGE_LIMIT + 1;
    }
    void execute() {
        if (pDB == nullptr) return;
        std::vector<Tuple*> ret;
        pDB->scan(startKey, endKey, ret);
    }
};


class Benchmark {
public:
    Benchmark() {}
    ~Benchmark() {}

    void Run() {
        auto start = chrono::high_resolution_clock::now(); 
        ios_base::sync_with_stdio(false); 
        
        runWorkload();
  
        auto end = chrono::high_resolution_clock::now(); 
        double time_taken =  
          chrono::duration_cast<chrono::milliseconds>(end - start).count(); 
        cout << "Time taken by program is : "
             // << time_taken << setprecision(9); 
             << time_taken;
        cout << " ms" << endl; 
    }

    // if we generate a random number and then check if cnt[rand] eq to 0, it could lead to 
    // huge overheads when only one single type Op is left( > 0)
    int getUncostlyRandomNum(int* cnts, int* codes) {
        // avoid using vector as memory allocation cost system calls, bring overheads as well
        int tmp[4] = {0, 0, 0, 0};
        int tmpSize = 0;
        for (int index = 0; index < 4; index++) 
            if (cnts[index] != 0) 
                tmp[tmpSize++] = index;

        if (tmpSize < 0 || tmpSize > 4) return -1;
        int randIndex = rand() % tmpSize;
        int randCodeIndex = tmp[randIndex];
        cnts[randCodeIndex]--;

        return codes[randCodeIndex];
    }

    OperationBase* getRandomOperation(int* cnts, int* codes){
        OperationBase* ret = nullptr;
        int selectedCode = getUncostlyRandomNum(cnts, codes);
        if (selectedCode == -1) return ret;

        switch (selectedCode)  {
            case OPERATION_CODE_GET :
            case OPERATION_CODE_DELETE:
                ret = new OperationGetDel(selectedCode);
                break;
            case OPERATION_CODE_SCAN:
                ret = new OperationScan(selectedCode);
                break;
            case OPERATION_CODE_PUT:
                ret = new OperationPut(selectedCode);
                break;
            default:
                "not catching any";
        }

        return ret;
    }

    void runWorkload() {
        int scanCnts = TOTAL_OPERATION * READ_PERCENT * SCAN_PERCENT;
        int getCnts = TOTAL_OPERATION * READ_PERCENT * GET_PERCENT;
        int putCnts = TOTAL_OPERATION * WRITE_PERCENT * PUT_PERCENT;
        int delCnts = TOTAL_OPERATION * WRITE_PERCENT * DEL_PERCENT;

        int cnts[4] = {getCnts, scanCnts, delCnts, putCnts};
        int codes[4] = {OPERATION_CODE_GET, OPERATION_CODE_SCAN, 
            OPERATION_CODE_DELETE, OPERATION_CODE_PUT};

        int opCnts = 0;
        while (std::accumulate(cnts, cnts+4, 0)) {
            // cout << "#" << opCnts << " operation starts ... \n";
            auto pOperation = getRandomOperation(cnts, codes);
            if (pOperation != nullptr) {
                pOperation->bindDB(db);
                pOperation->execute();

                delete pOperation;
            } else {
                // cout << "ptr is null! \n";
            }

            opCnts++;
        }
    }

    templatedb::DB db;
};

int main(int argc, char **argv)
{
    srand(time(0));
    Benchmark* pSuite = new Benchmark();
    pSuite->Run();
    delete pSuite;

    return 0;
}