#include "store.h"

Store::Store(){
	storage.resize(32);
}

std::bitset<32> Store::fetch_line(int line){
	return storage[line];
}