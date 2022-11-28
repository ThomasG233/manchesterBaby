#include "processor.h"

Processor::Processor(Store *stPtr){
	accumulator.reset();
	control_instruction.reset();
	present_instruction.reset();

	store = stPtr;
}

//return integer representation of control instruction register
int Processor::get_ci(){
	return (int)control_instruction.to_ulong();
}

//increment control instruction register and reset operand/opcode values
void Processor::incr_ci(){
	//first step in fetch execute cycle
	int tmp = (int)control_instruction.to_ulong();
	tmp++;
	control_instruction = tmp;

	//reset opcode and operand variables
	operand = 0;
	opcode = 0;
}

//fetch line of store pointed to by control instruction
void Processor::fetch(){
	present_instruction = store->fetch_line(get_ci());
}

//decode present instruction into operand and opcode pair
void Processor::decode(){

	//significant bits in present instruction register
	//bits 0-4 specify operand, bits 13-15 specify opcode
	int index[8] = {0,1,2,3,4,13,14,15};

	std::bitset<5> operand;
	std::bitset<3> opcode;
	std::bitset<5> operand_fetch_value;

	for(int i = 0; i < 8; i++){
		if(i >= 0 && i <= 4){
			operand<<present_instruction[index[i]];
		}
		else{
			opcode<<present_instruction[index[i]];
		}
	}
}



