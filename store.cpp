#include "store.h"

Store::Store(){
	storage.resize(32);
}

std::bitset<32> Store::fetch_line(int line){
	return storage[line];
}

void Store::set_line(std::bitset<32> newStorage, int line){
	storage[line] = newStorage;
}
