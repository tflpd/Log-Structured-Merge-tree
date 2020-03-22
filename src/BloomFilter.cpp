#include "BloomFilter.h"
#include "Murmurhash.h"

using namespace std;
using namespace BF;

BloomFilter::BloomFilter(){
	numElement = 1024;
	bitsPerElement = 10;

	numIndex = (int)floor(0.693*bitsPerElement+ 0.5);
	size = numElement * bitsPerElement;
}

BloomFilter::BloomFilter( int numElement_, int bitsPerElement_ ){
	numElement = numElement_;
	bitsPerElement = bitsPerElement_;
	numIndex = (int)floor(0.693*bitsPerElement+ 0.5);
	size = numElement * bitsPerElement;

	makeBloomFilter();
}

void BloomFilter::makeBloomFilter(){
	bf_vec.resize(size, 0);
}

void BloomFilter::program( string key ){
	vector<int> index( numIndex, 0 );
	getIndex( key, &index );

	for(int i=0; i<numIndex; i++){
		bf_vec[index[i]] = 1;
	}
}

bool BloomFilter::query( string key ){
	vector<int> index( numIndex, 0 );
	getIndex( key, &index );

	for(int i=0; i<numIndex; i++){
		if( bf_vec[index[i]] == 0 )
			return false;
	}

	return true; // positive
}

void BloomFilter::getIndex( string key, vector<int>* index ){
	unsigned int h = MurmurHash2( key.c_str(), key.size(), 0xbc9f1d34 );

	const unsigned int delta = (h>>17) | (h<<15); // Rorate right 17 bits
	for( int i=0 ; i<numIndex ; i++ ){
		index->at(i) = h % size;
		h += delta;
	}

	return;
}

int BloomFilter::getIndexNum(){
	return numIndex;
}

int BloomFilter::getSize(){
	return size;
}

int BloomFilter::getNumElement() {
    return numElement;
}
