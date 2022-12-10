#include "store.h"
#include <iostream>

Store::Store(){
	storage.resize(32);
}
// resizes the storage capacity
void Store::resizeStorage(int userSize){
	storage.resize(userSize);
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
int Store::load_file(std::string file){
	//open file
	std::ifstream input;
	int memorySize = storage.size();
	input.open(file);

	if(input.is_open())  //check if the file can be opened
	{
		std::string line;
		bool fileRight = true;
		int lineCount = 0;
		int lineSize = 0;

		while(getline(input,line) && fileRight == true)
		{
			lineSize = line.length();
			int numberCount = 0;
			if(lineCount > memorySize)
			{
				fileRight = false;
			}

			if(lineSize != 32) // if file doesnt contain a line that does not equal 32 bits 
			{
				fileRight = false;
			}
			else
			{

				for(int i = 0; i < lineSize; i++)
				{
					if(lineCount < memorySize)
					{
						if(line[i] == '0' || line[i] == '1')
						{
							numberCount++;
						}
						else
						{
							fileRight = false;
						}
					}
				}
			}
			if(fileRight == true)
			{
				line.assign(line, 0, 32);
				storage[lineCount] = std::bitset<32>(line);
				lineCount++;
			}
		}
		input.close();

		if(lineCount > memorySize)
		{ 
			return EXCEEDED_LENGTH;
		}
		if(lineSize != 32)
		{
			return BITSET_TOO_LONG;
		}
		if(fileRight == false)
		{
			return INVALID_CONTENT;
		}
		return SUCCESS;
	}
	else 
	{
		return FILE_NOT_FOUND;
	}
}
// return the size of the store
int::Store::get_store_size(){
	return storage.size();
}
