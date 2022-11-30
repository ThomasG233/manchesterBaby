#include "processor.h"
#include <algorithm>
#include <iostream>
#include <string>

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

	for(int i = 0; i < 8; i++){
		if(i >= 0 && i <= 4){
			operand<<present_instruction[index[i]];
		}
		else{
			opcode<<present_instruction[index[i]];
		}
	}
}

void Processor::execute(){
	//reverse opcode and operand
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
	control_instruction = store->fetch_line(get_ci());
}
void Processor::JRP(){
	// converts both values to string
	std::string accVal = control_instruction.to_string();
	std::string storVal = (store->fetch_line(get_ci())).to_string();
	// flips for left as most-significant-bit format
	reverse(accVal.begin(), accVal.end());
	reverse(storVal.begin(), storVal.end());
	// carry out the subtraction
	int result = (std::bitset<32>(accVal)).to_ulong() + (std::bitset<32>(storVal).to_ulong());
	std::string finalVal = std::bitset<32>(result).to_string();
	// reverse this to put back into normal format before changing accumulator
	reverse(finalVal.begin(), finalVal.end());
	accumulator = std::bitset<32>(finalVal);
}
void Processor::LDN(){
	
}
void Processor::STO(){
	store->set_line(accumulator, get_ci());
}
void Processor::SUB(){
	// converts both values to string
	std::string accVal = accumulator.to_string();
	std::string storVal = (store->fetch_line(get_ci())).to_string();
	// flips for left as most-significant-bit format
	std::reverse(accVal.begin(), accVal.end());
	std::reverse(storVal.begin(), storVal.end());
	// carry out the subtraction
	int result = (std::bitset<32>(accVal)).to_ulong() - (std::bitset<32>(storVal).to_ulong());
	std::string finalVal = std::bitset<32>(result).to_string();
	// reverse this to put back into normal format before changing accumulator
	std::reverse(finalVal.begin(), finalVal.end());
	accumulator = std::bitset<32>(finalVal);
}
void Processor::CMP(){
	if(accumulator.to_ulong() < 0)
	{
		incr_ci();
	}
}
void Processor::STP(){
	stopLamp = true;
}
