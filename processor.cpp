#include "processor.h"
#include <algorithm>
#include <iostream>
#include <string>

/*
only numbers to be stored and read in 2s complement are numbers stored in accumulator and stored in vars section of the machine code
	-when reading numbers from store, convert to 2s complement form 
	-when reading number from accumulator, read based on fact that it is in 2s complement form
	-CI and PI do not need 2s complement as they are simply references to the store
*/

Processor::Processor(Store *stPtr){
	accumulator.reset();
	control_instruction.reset();
	present_instruction.reset();
	stopLamp = false;
	store = stPtr;
}

//return integer representation of control instruction register
int Processor::get_ci(){
	return (int)control_instruction.to_ulong();
}

bool Processor::get_stop(){
	return stopLamp;
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

	operand = std::bitset<5>(present_instruction.to_string().substr(0, 5));
	
	opcode = std::bitset<3>(present_instruction.to_string().substr(13, 3));
	
}

void Processor::execute(){
	//reverse opcode and operand as it is in reverse in the machine code file
	//convert to string
	std::string strOpcode = opcode.to_string();
	std::string strOperand = operand.to_string();

	//reverse string
	reverse(strOpcode.begin(), strOpcode.end());
	reverse(strOperand.begin(), strOperand.end());

	//convert string to bitset
	opcode = std::bitset<3>(strOpcode);
	operand = std::bitset<5>(strOperand);

	//call appropriate function based upon opcode
	(this->*opcodes[(int)opcode.to_ulong()])();
}

void Processor::JMP(){
	//copy contents of store line to CI, contents is the variable stored at the operand
	control_instruction = store->fetch_line((int)operand.to_ulong());
}
void Processor::JRP(){
	//jump relative, add contents of store line to CI
	//should grab the variable stored at operand and add that to the CI
	// for example this instruction could jump ahead two lines, JRP 2
	control_instruction = store->fetch_line(get_ci() + (int)operand.to_ulong());
}
void Processor::LDN(){
	//copy content of store line to accumulator, negated
	//function needs to be finished

	accumulator = store->fetch_line((int)operand.to_ulong());

	accumulator.flip();
	accumulator = std::bitset<32>((int)accumulator.to_ulong() + 1);

}
void Processor::STO(){
	//copy content of accumulator to store line - THIS NEEDS FIXED - should point to the variables section of store not the 
	//instructions part (i.e past the END: bit), should store accumulator to the line pointed to by operand
	store->set_line(accumulator, (int)operand.to_ulong());
}
void Processor::SUB(){
	//subtract content of Store line from accumulator
	//should subtract content at line pointed to by operand from the accumulator
	int result = 0;
	int accNum = 0;
	int storeNum = 0;
	// grab value of accumulator
	std::string acc = accumulator.to_string();
	reverse(acc.begin(), acc.end());
	// if value on accumulator is negative
	if(acc[0] == '1')
	{
		std::bitset<32> twosCompConversion = accumulator.flip();
		accNum = ((int)(twosCompConversion.to_ulong()) + 1) * -1;
	}
	else // do normal conversion
	{
		accNum= (int)((std::bitset<32>(acc)).to_ulong());
	}
	// grab value of store location
	std::string storeValue = (store->fetch_line(operand.to_ulong())).to_string();
	storeValue = storeValue.assign(storeValue, 27, 5);
	reverse(storeValue.begin(), storeValue.end());
	int storeNum = (int)((std::bitset<5>(acc)).to_ulong());
	// get result
	result = accNum - storeNum;
	// set accumulator to new value
	if(result < 0)
	{
		accumulator = (std::bitset<32>(-1 * (result - 1))).flip();
	}
	else
	{
		accumulator = std::bitset<32>(result);
	}
}
void Processor::CMP(){
	// if the number is negative
	if(accumulator.test(31))
	{
		incr_ci();
	}
}
void Processor::STP(){
	stopLamp = true;
}
