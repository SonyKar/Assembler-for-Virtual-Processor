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
	public:
		static const short int QUATER_LEN = 4;
		static const short int HALF_LEN = 8;
		static const short int FULL_LEN = 16;

		Transform();

		string GetInstructionBinaryCode(string instruction);
		string IntToBinary(int value, short int operandLength);
		string BinaryLineToHex(string binary);
	};
}
