#include "store.h"

Store::Store(){
	storage.resize(32);
}

//return line at line
std::bitset<32> Store::fetch_line(int line){
	return storage[line];
}

//set line at line
void Store::set_line(std::bitset<32> acc, int ci){
	storage[ci] = acc;
}

//load machine code into store
void Store::load_file(std::string file){
	//open file
	std::ifstream mc;
	mc.open(file);

	//every line is set as bitset in store vector (up to a max of 32 lines)
	std::string line;
	int count = 0;
	while(std::getline(mc, line)){
		line.assign(line, 0, 32);
		storage[count] = std::bitset<32>(line);
		count++;
	}

	//close file
	mc.close();
}
int::Store::get_store_size(){
	return storage.size();
}
