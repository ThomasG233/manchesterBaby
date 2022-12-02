#include "store.h"

Store::Store(){
	storage.resize(32);
}

std::bitset<32> Store::fetch_line(int line){
	return storage[line];
}

void Store::set_line(std::bitset<32> acc, int ci){
	storage[ci] = acc;
}

void Store::load_file(std::string file){
	std::ifstream mc;
	mc.open(file);

	std::string line;
	int count = 0;
	while(std::getline(mc, line)){

		line.erase(std::remove(line.begin(), line.end(), '\r'), line.cend());

		storage[count] = std::bitset<32>(line);
		count++;
	}
}