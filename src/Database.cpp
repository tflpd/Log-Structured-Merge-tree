#include "Database.hpp"
#include "Args.h"

using namespace templatedb;

DB::DB() {
    auto sstSize = getSSTSize();
    auto tupleSize = getTupleBytesSize(); // only 1 key, at most 10 values
    auto a = getA();
    auto runs = getMaxRunsBeforeMerge();
    table = new LSM_T(sstSize, tupleSize, a, runs);
}

DB::~DB() {
    delete table;
}


Value DB::get(int key)
{
    Value ret;
    return ret;
}


void DB::put(int key, Value val)
{
    table->Insert(to_string(key), val);
}


std::vector<Value> DB::scan()
{
   std::vector<Value> return_vector;
   // for (auto pair: table)
   // {
   //     return_vector.push_back(pair.second);
   // }

   return return_vector;
}


std::vector<Value> DB::scan(int min_key, int max_key)
{
    std::vector<Value> return_vector;
    // auto tuples = table->Search(min_key, max_key);

    return return_vector;
}


void DB::del(int key)
{
//    table->Delete(key);
}


void DB::show_buf() const
{
    table->ShowMemBuffer();
}


std::vector<Value> DB::execute_op(Operation op)
{
    std::vector<Value> results;
    if (op.type == GET)
    {
        results.push_back(this->get(op.key));
    }
    else if (op.type == PUT)
    {
        this->put(op.key, Value(op.args));
    }
    else if (op.type == SCAN)
    {
        results = this->scan(op.key, op.args[0]);
    }
    else if (op.type == DELETE)
    {
        this->del(op.key);
    }

    return results;
}


// bool DB::load_data_file(std::string & fname)
// {
//     std::ifstream fid(fname);
//     if (fid.is_open())
//     {
//         int key;
//         std::string line;
//         std::getline(fid, line); // First line is rows, col
//         while (std::getline(fid, line))
//         {
//             std::stringstream linestream(line);
//             std::string item;

//             std::getline(linestream, item, ',');
//             key = stoi(item);
//             std::vector<int> items;
//             while(std::getline(linestream, item, ','))
//             {
//                 items.push_back(stoi(item));
//             }
//             this->put(key, Value(items));
//         }
//     }
//     else
//     {
//         fprintf(stderr, "Unable to read %s\n", fname.c_str());
//         return false;
//     }

//     return true;
// }


// db_status DB::open(std::string & fname)
// {
//     this->file.open(fname, std::ios::in | std::ios::out);
//     if (file.is_open())
//     {
//         this->status = OPEN;
//         // New file implies empty file
//         if (file.peek() == std::ifstream::traits_type::eof())
//             return this->status;

//         int key;
//         std::string line;
//         std::getline(file, line); // First line is rows, col
//         while (std::getline(file, line))
//         {
//             std::stringstream linestream(line);
//             std::string item;

//             std::getline(linestream, item, ',');
//             key = stoi(item);
//             std::vector<int> items;
//             while(std::getline(linestream, item, ','))
//             {
//                 items.push_back(stoi(item));
//             }
//             this->put(key, Value(items));
//             if (value_dimensions == 0)
//                 value_dimensions = items.size();
//         }
//     }
//     else if (!file) // File does not exist
//     {
//         this->file.open(fname, std::ios::out);
//         this->status = OPEN;
//     }
//     else
//     {
//         file.close();
//         this->status = ERROR_OPEN;
//     }

//     return this->status; 
// }


// bool DB::close()
// {
//     if (file.is_open())
//     {
//         this->write_to_file();
//         file.close();
//     }
//     this->status = CLOSED;

//     return true;
// }


// bool DB::write_to_file()
// {
//     file.clear();
//     file.seekg(0, std::ios::beg);

//     std::string header = std::to_string(table.size()) + ',' + std::to_string(value_dimensions) + '\n';
//     file << header;
//     for(auto item: table)
//     {
//         std::ostringstream line;
//         std::copy(item.second.items.begin(), item.second.items.end() - 1, std::ostream_iterator<int>(line, ","));
//         line << item.second.items.back();
//         std::string value(line.str());
//         file << item.first << ',' << value << '\n';
//     }

//     return true;
// }