#include <iostream>
#include <algorithm>
#include "processor.h"


int main(){

	std::string response = "";
	// Collect information related to Baby's storage size.
	std::cout << "By default, the Manchester Baby holds 32 16-bit numbers in its storage. You can choose to extend this if you wish.\nWould you like to extend it? (Y/N): ";
	std::cin >> response;
	Store store;
	// If the user wants to resize the storage.
	if(response == "Y" || response == "y")
	{
		std::cout << "Enter the new amount of lines to have in storage (minimum of 32, maximum of 64): ";
		std::cin >> response;
		// Try-catch block, in case of a failure to convert string
		try
		{
			int value = stoi(response);
			// If the value entered is valid.
			if(value > 32 && value < 65)
			{
				std::cout << "The storage will have " << value << " lines of storage." << std::endl;
				store.resizeStorage(value);
			}
			else
			{
				std::cout << "Invalid amount; the storage will remain at its base size of 32 lines." << std::endl;
			}
		}
		catch(...)
		{
			std::cout << "Invalid data provided; the storage will remain at its base size of 32 lines." << std::endl;
		}
	}
	Processor processor(&store);

	std::cout << "Enter the file you wish to read in the machine code from: ";
	std::cin >> response;

	store.load_file(response);
	while(!processor.get_stop()){

		//std::cout<<"incrementing CI" << std::endl;
		processor.incr_ci();

		//std::cout<<"fetch"<<std::endl;
		processor.fetch();

		//std::cout<<"decode"<<std::endl;
		processor.decode();

		//std::cout<<"execute"<<std::endl;
		processor.execute();
		
		//std::cout<<"printing"<<std::endl;
		processor.printout();
	}

	/*-------- DEBUGGING ------


	*/
	//printing answer in decimal form

	std::bitset<32> answer = store.fetch_line(9);
	if(answer[0] == 0){
		std::string ansStr = answer.to_string();
		reverse(ansStr.begin(), ansStr.end());
		answer = std::bitset<32>(ansStr);
		std::cout << "Answer: " << answer.to_ulong() << std::endl;
	}
	else{
		answer.flip();

		std::string ansStr = answer.to_string();
		reverse(ansStr.begin(), ansStr.end());
		answer = std::bitset<32>(ansStr);
		answer = std::bitset<32>(answer.to_ulong() + 1);

		std::cout << "Answer: " << answer.to_ulong() << std::endl;
	}
	
	return 0;

}
