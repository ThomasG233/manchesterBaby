#include <iostream>
#include "processor.h"


int main(){
	Store store;
	Processor processor(&store);

	store.load_file("BabyTest1-MC.txt");
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