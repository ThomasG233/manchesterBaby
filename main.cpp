#include <iostream>
#include "processor.h"


int main(){
	Store store;
	Processor processor(&store);

	store.load_file("BabyTest1-MC.txt");
	while(!processor.get_stop()){

		std::cout<<"incrementing CI" << std::endl;
		processor.incr_ci();

		std::cout<<"fetch"<<std::endl;
		processor.fetch();

		std::cout<<"decode"<<std::endl;
		processor.decode();

		std::cout<<"execute"<<std::endl;
		processor.execute();
		
		std::cout<<"printing"<<std::endl;
		processor.printout();
	}
	
	return 0;

}
