#include <iostream>
#include <chrono>

#include "include/BloomFilter.h"

using namespace BF;

void queryTest( BloomFilter *bf,  string key ){
	bool result = bf->query( key );
	string result_str = (result)? "positive" : "negative";
	cout << "Bloom Filter query result of " << key  << " is " << result_str << endl;
}

int main(int argc, char * argv[])
{
    BloomFilter bf( 1024, 10 ); // number of keys, bits per element

    // Enpty filter is not match, at this level
    queryTest(&bf, "hello");
    queryTest(&bf, "world");

    // insert two string
    bf.program("hello");
    bf.program("world");

    queryTest(&bf, "hello");
    queryTest(&bf, "world");
    queryTest(&bf, "x");
    queryTest(&bf, "foo");

    return 0;
}
