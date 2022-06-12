#include <string>
#include <map>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

#pragma once
namespace assembler 
{
	class Transform
	{
	private:
		map<string, string> instructionDictionary;
		static const string IMEDIATE_ADDRESS;
		static const string DIRECT_REGISTER;
		static const string INDIRECT_REGISTER;
		static const string INDEX_ADDRESS;

		int StringToInt(string str, int startIndex, int endIndex);
	public:
		static const short int QUATER_LEN = 4;
		static const short int HALF_LEN = 8;
		static const short int FULL_LEN = 16;

		Transform();

		string GetInstructionBinaryCode(string instruction);
		int GetInstructionType(string opcode);
		string IntToBinary(int value, short int operandLength);
		wchar_t BinaryLineToByte(string binary);
		string OperandToBinary(string operand, short int length);
	};
}
