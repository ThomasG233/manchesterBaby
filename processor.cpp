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

//return state of stopLamp
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

void Processor::printout(){
	// print out the status of the machine at this point of the code
	// NOTE; all of the printouts are in Right-Most-Bit format.
	std::bitset<32> bits;
	std::string bitset;
	// ACCUMULATOR PRINTOUT: For debug purposes
	std::cout << "A: ";
	bitset = accumulator.to_string();
	reverse(bitset.begin(), bitset.end());
	bits = std::bitset<32>(bitset);
	// print out each bit, full square for 1, empty for 0
	for(int j = 0; j < 32; j++)
	{
		if(bits.test(j))
		{
			std::cout << "■ ";
		}
		else
		{
			std::cout << "□ ";
		}
	}

	std::cout << "\n" <<std::endl;
	std::cout << "C: "; 
	bitset = control_instruction.to_string();
	reverse(bitset.begin(), bitset.end());
	bits = std::bitset<32>(bitset);
	// print out each bit, full square for 1, empty for 0
	for(int j = 0; j < 32; j++)
	{
		if(bits.test(j))
		{
			std::cout << "■ ";
		}
		else
		{
			std::cout << "□ ";
		}
	}
	std::cout << "\n" << std::endl;
	// STORE PRINTOUT:
	// for each line of the store
	for(int i = 0; i < 32; i++)
	{
		std::cout << i << ": ";

		//print extra space to keep all lines in line
		if(i <= 9){
			std::cout << " ";
		}

		// print out in right-most-bit format
		bitset = (store->fetch_line(i)).to_string();
		reverse(bitset.begin(),bitset.end());
		bits = std::bitset<32>(bitset);
		for(int j = 0; j < store->get_store_size(); j++)
		{
			if(bits.test(j))
			{
				std::cout << "■ ";
			}
			else
			{
				std::cout << "□ ";
			}
		}
		std::cout << std::endl;
	}
}

//OPCODE 000
void Processor::JMP(){
	//jump to line number in operand
	std::cout << "JMP " << operand.to_ulong() << std::endl;
	int operand_value = (int)operand.to_ulong(); 
	control_instruction = std::bitset<32>(operand_value);
}

//OPCODE 10
void Processor::JRP(){
	//jump relative, add contents of store line to CI
	std::cout << "JRP " << operand.to_ulong() << std::endl; 
	control_instruction = store->fetch_line(get_ci() + (int)operand.to_ulong());
}

//OPCODE 010
void Processor::LDN(){
	//copy content of store line to accumulator, negated (2s complement)
	
	std::cout << "LDN " << operand.to_ulong() << std::endl;

	//fetch operand line
	accumulator = store->fetch_line((int)operand.to_ulong());

	//flip the bits
	accumulator.flip();

	//reverse
	std::string value = accumulator.to_string();
	reverse(value.begin(), value.end());
	accumulator = std::bitset<32>(value);

	//add 1 to accumulator
	accumulator = std::bitset<32>(((int)accumulator.to_ulong() + 1));

	//reverse again to maintain big-endian format
	value = accumulator.to_string();
	reverse(value.begin(), value.end());
	accumulator = std::bitset<32>(value);

}

//OPCODE 110
void Processor::STO(){
	//copy content of accumulator to store line
	std::cout << "STO " << operand.to_ulong() << std::endl;
	store->set_line(accumulator, (int)operand.to_ulong());
}

//OPCODE 001 & 101
void Processor::SUB(){
	//subtract content of Store line from accumulator
	//should subtract content at line pointed to by operand from the accumulator
	std::cout << "SUB " << operand.to_ulong() << std::endl;
	int result = 0;
	int accNum = 0;
	int storeNum = 0;

	// grab value of accumulator and reverse order of bits
	std::string acc = accumulator.to_string();
	reverse(acc.begin(), acc.end());
	accumulator = std::bitset<32>(acc);

	// if value on accumulator is negative - convert from 2s complement format to decimal
	if(acc[31] == '1')
	{
		std::bitset<32> twosCompConversion = accumulator;
		twosCompConversion.flip();
		accNum = ((int)(twosCompConversion.to_ulong()) + 1) * -1;
	}
	else // do normal conversion
	{
		accNum = (int)accumulator.to_ulong();
	}

	// grab value of store location
	std::string storeValue = (store->fetch_line(operand.to_ulong())).to_string();
	reverse(storeValue.begin(), storeValue.end());
	storeNum = (int)((std::bitset<32>(storeValue)).to_ulong());

	// get result
	result = accNum - storeNum;

	// set accumulator to new value
	//if result is negative, store in 2s complement form
	if(result < 0)
	{
		//store positive form of number to accumulator
		accumulator = std::bitset<32>(result*-1);

		//flip the bits
		accumulator.flip();

		//add one 
		accumulator = std::bitset<32>((int)accumulator.to_ulong() + 1);

		//reverse order of bits to maintain big-endianness
		acc = accumulator.to_string();
		reverse(acc.begin(), acc.end());
		accumulator = std::bitset<32>(acc);


	}
	//if result is positive, store normally
	else
	{
		//reverse order of bits to maintain big-endianness and give to accumulator
		accumulator = std::bitset<32>(result);
		acc = accumulator.to_string();
		reverse(acc.begin(), acc.end());
		accumulator = std::bitset<32>(acc);
	}
}

//OPCODE 011
void Processor::CMP(){
	// if the number is negative, skip next instruction
	std::cout << "CMP " << std::endl;
	if(accumulator.test(31))
	{
		incr_ci();
	}
}

//OPCODE 111
void Processor::STP(){
	//set stop lamp to true
	std::cout << "STP " << std::endl;
	stopLamp = true;
}