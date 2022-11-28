#include <iostream>
#include "processor.h"


int main(){
	Store store;
	Processor processor(&store);

	for(int i = 0; i < 100; i++){
		std::cout << processor.get_ci() << std::endl;
		processor.incr_ci();
	}

	return 0;

}