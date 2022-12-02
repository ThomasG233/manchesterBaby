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
	// converts binary to string format
	std::string storeVal = (store->fetch_line(get_ci())).to_string();
	std::string conOperand;
	std::string storeOperand;
	// collect operands from binary
	(control_instruction.to_string()).assign(conOperand, 27, 31);
	storeVal.assign(storeOperand, 27, 31);
	// reverse for correct notation
	reverse(conOperand.begin(), conOperand.end());
	reverse(storeOperand.begin(), storeOperand.end());
	// carry out the addition
	int result = (std::bitset<5>(conOperand).to_ulong()) + (std::bitset<5>(storeOperand).to_ulong());
	// if value is negative, change negative bit to reflect this
	std::string resultBitset;
	if(result < 0)
	{
		storeVal[26] = '1';
		resultBitset = std::bitset<5>(-(result)).to_string();
	}
	else
	{
		storeVal[26] = '0';
		resultBitset = std::bitset<5>(result).to_string();
	}
	// update the store binary to account for the new operand
	reverse(resultBitset.begin(), resultBitset.end());
	for(int i = 0; i < 5; i++)
	{
		storeVal[27+i] = resultBitset[i];
	}
	// update Ci to new value
	control_instruction = std::bitset<32>(storeVal);
}
void Processor::LDN(){
	
}
void Processor::STO(){
	store->set_line(accumulator, get_ci());
}
void Processor::SUB(){
	// converts binary to string format
	std::string storeVal = (store->fetch_line(get_ci())).to_string();
	std::string accOperand;
	std::string storeOperand;
	// collect operands from the store and the accumulator
	(accumulator.to_string()).assign(accOperand, 27, 31);
	storeVal.assign(storeOperand, 27, 31);
	// reverse for correct notation
	reverse(accOperand.begin(), accOperand.end());
	reverse(storeOperand.begin(), storeOperand.end());
	// carry out the subtraction, then convert to bitset
	int result = (std::bitset<5>(accOperand).to_ulong()) - (std::bitset<5>(storeOperand).to_ulong());
	std::string resultBitset;
	if(result < 0)
	{
		storeVal[26] = '1';
		resultBitset = std::bitset<5>(-(result)).to_string();
	}
	else
	{
		storeVal[26] = '0';
		resultBitset = std::bitset<5>(result).to_string();
	}	
	// update the store binary to account for new operand
	reverse(resultBitset.begin(), resultBitset.end());
	for(int i = 0; i < 5; i++)
	{
		storeVal[27+i] = resultBitset[i];
	}
	// update accumulator to new value
	accumulator = std::bitset<32>(storeVal);
}
void Processor::CMP(){
	// if the number is negative
	if(accumulator.test(26))
	{
		incr_ci();
	}
}
void Processor::STP(){
	stopLamp = true;
}
