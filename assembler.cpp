//Manchester Baby Simulator assembler
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <map>
#include <iomanip>
#include <bitset>


using namespace std;

//Create a symbol table
map<string, string> symbolTable;

//Create an instruction set
map<string, string> instructionSet;

//Create a variable map
map<string, string> variableMap;

//Create an output code buffer
vector<string> outputCodeBuffer;

//Create the instruction set
//Any further instructions, place in here with their machine code counterpart
void createInstructionSet()
{
	instructionSet["JMP"] = "000";
	instructionSet["JRP"] = "100";
	instructionSet["LDN"] = "010";
	instructionSet["SUB"] = "001";
	instructionSet["STO"] = "110";
	instructionSet["STP"] = "111";
	instructionSet["CMP"] = "011";
}

//Create the variable table
//Work to make this dynamic? I want to be able to read in any variable names. 
void createVariableTable()
{
	variableMap["NUM01"] = "11100";
	variableMap["NUM02"] = "00010";
	variableMap["MYSUM"] = "10010";
}

//Function to add label and address to top of symbol table
void addToSymbolTable(string symbol, string address)
{
	symbolTable[symbol] = address;
}

//Function to add to outputCodeBuffer
void addToOutputCodeBuffer(string instruction)
{
	//Because of how vectors work, this should format it correctly.
	outputCodeBuffer.push_back(instruction);
}

//Take input file, clean file and then place contents into a vector
//Try add more validation?
void cleanFile(string fileName, vector<string>& workedFile)
{
	//Open file
	ifstream file(fileName);
	//Check if file is open
	if (file.is_open())
	{
		//Read file line by line
		string line;
		while (getline(file, line))
		{
			//Remove comments
			line = line.substr(0, line.find(";"));
			//Remove whitespace
			line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
			//Add line to vector
			workedFile.push_back(line);
		}
		//If worked file is empty, throw error
		if (workedFile.empty())
		{
			cout << "Error: File is empty" << endl;
		}
	}
	else
	{
		cout << "Error: File not found" << endl;
	}
}

//Make a first pass of the cleaned file (try make a second pass?)
void firstPass(vector<string> workedFile)
{
	//Create a string to keep track of the address, the address is formatted as such: $000 as a hexadecimal
	string address = "$";
	int addressCounter = 0;
	//Iterate through the worked file
	for (int i = 0; i < ((int)workedFile.size()); i++)
	{
		//Check if the line is a label
		if (workedFile[i].find(":") != string::npos && workedFile[i].find("VAR") == string::npos)
		{
			//Get the label
			string label = workedFile[i].substr(0, workedFile[i].find(":"));
			//Add the label and address to the symbol table
			addToSymbolTable(label, address);
		}
		//Check if the line is an instruction
		if (workedFile[i].find("LDN") != string::npos || workedFile[i].find("SUB") != string::npos || workedFile[i].find("STO") != string::npos || workedFile[i].find("STP") != string::npos || workedFile[i].find("CMP") != string::npos || workedFile[i].find("JMP") != string::npos || workedFile[i].find("JRP") != string::npos)
		{
			//Get the instruction (There's a commented ot snippet depending on if we want to do an if check here as more accurate later in the code?)u
			string instruction = workedFile[i].substr(workedFile[i].find(":") + 1, 3);
			//Get the operand
			string operand = workedFile[i].substr(workedFile[i].find(":") + 4, workedFile[i].length());
			//Convert the operand to the machine code via the variable map
			string machineCode = variableMap[operand];
			//If operand is empty, we need to fill the earlier spaces with 0's
			if (operand.empty())
			{
				machineCode = "00000";
			}
			//Add 8 0's to the machinecode (seperation, idk man it's the output looked like lmao)
			machineCode += "00000000";
			//Convert the instruction to machine code
			machineCode += instructionSet[instruction];
			//Fill the rest of the line with 0's 
			machineCode += "0000000000000000";
			//Add the machine code to the output code buffer
			addToOutputCodeBuffer(machineCode);
			//Increment the address counter
			addressCounter++;
			//Convert the addressCounter to hexadecimal
			stringstream stream;
			stream << hex << addressCounter;
			string result(stream.str());
			//Add the address to the address string (REWORK THIS!!!!!!)
			address = "$" + result;
		}
		//If the line is a variable, convert the variable into binary and place at start of output code buffer
		if (workedFile[i].find("VAR") != string::npos)
		{
			//Get the variable
			string variable = workedFile[i].substr(workedFile[i].find("VAR") + 3, workedFile[i].length());
			//Convert the variable to binary
			string binary = bitset<32>(stoi(variable)).to_string();
			//Reversing the string so it goes to the start of the output code buffer
			reverse(binary.begin(), binary.end());
			//Add the binary to the output code buffer
			addToOutputCodeBuffer(binary);
			//Increment the address counter
			addressCounter++;
			//Convert the addressCounter to hexadecimal
			stringstream stream;
			stream << hex << addressCounter;
			string result(stream.str());
			//Add the address to the address string
			address = "$" + result;
		}
	}
	cout << "Symbol Table" << endl;
	for (auto const& x : symbolTable)
	{
		cout << x.first << " " << x.second << endl;
	}
	cout << "Output Code Buffer" << endl;
	for (int i = 0; i < outputCodeBuffer.size(); i++)
	{
		cout << outputCodeBuffer[i] << endl;
	}
	cout << "Done!" << endl;
	//Save the codebuffer to an output file (Make optional?)
	ofstream outputFile("output.txt");
	for (int i = 0; i < outputCodeBuffer.size(); i++)
	{
		outputFile << outputCodeBuffer[i] << endl;
	}
	cout << "Output file saved" << endl;
}

/*
Use if statements to determine instruction in first pass
if (workedFile[i].find("LDN") != string::npos)
			{
				instruction = "LDN";
			}
			else if (workedFile[i].find("SUB") != string::npos)
			{
				instruction = "SUB";
			}
			else if (workedFile[i].find("STO") != string::npos)
			{
				instruction = "STO";
			}
			else if (workedFile[i].find("STP") != string::npos)
			{
				instruction = "STP";
			}
			else if (workedFile[i].find("CMP") != string::npos)
			{
				instruction = "CMP";
			}
			else if (workedFile[i].find("JMP") != string::npos)
			{
				instruction = "JMP";
			}
			else if (workedFile[i].find("JRP") != string::npos)
			{
				instruction = "JRP";
			}
*/

//Main method
int main()
{
	//Creates the instruction set
	createInstructionSet();
	//Creates the variable map
	createVariableTable();
	//Create vector to hold cleaned file
	vector<string> workedFile;
	//Create vector to hold binary file (archaic?)
	//vector<string> binaryFile;
	//Use user input to get file name
	string fileName;
	cout << "Enter file name: ";
	cin >> fileName;
	cleanFile(fileName, workedFile);
	//Convert to binary
	firstPass(workedFile);
	return 0;
}
