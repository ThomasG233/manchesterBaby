#include <bitset>
#include "store.h"

class Processor{
	private:
		std::bitset<32> accumulator;
		std::bitset<32> control_instruction;
		std::bitset<32> present_instruction;

		std::bitset<5> operand;
		std::bitset<3> opcode;

		bool stopLamp;

		Store *store;

		typedef void(Processor::*instructions)();
		instructions opcodes[8] = {&Processor::JMP, &Processor::JRP, &Processor::LDN, &Processor::STO,
				&Processor::SUB, &Processor::SUB, &Processor::CMP, &Processor::STP};

	public:
		Processor(Store *stPtr);

		//returns integer representation of ci
		int get_ci();

		bool get_stop();

		//fetch-execute cycle functions
		void incr_ci();
		void fetch();
		void decode();
		void execute();
		void printout();

		//opcode functions
		void JMP();
		void JRP();
		void LDN();
		void STO();
		void SUB();
		void CMP();
		void STP();

};
