#include "pch.h"
#include "Transform.h"

using namespace assembler;

Transform::Transform()
{
	instructionDictionary = {
		// b1
		{"mov", "0000"},
		{"add", "0001"},
		{"sub", "0010"},
		{"cmp", "0011"},
		{"and", "0100"},
		{"or", "0101"},
		{"xor", "0110"},

		// b2
		{"clr", "1000000000"},
		{"neg", "1000000001"},
		{"inc", "1000000010"},
		{"dec", "1000000011"},
		{"asl", "1000000100"},
		{"asr", "1000000101"},
		{"lsr", "1000000110"},
		{"rol", "1000000111"},
		{"ror", "1000001000"},
		{"rlc", "1000001001"},
		{"rrc", "1000001010"},
		{"jmp", "1000001011"},
		{"call", "1000001100"},
		{"push", "1000001101"},
		{"pop", "1000001110"},

		// b3
		{"br", "11000000"},
		{"bne", "11000001"},
		{"beq", "11000010"},
		{"bpl", "11000011"},
		{"bmi", "11000100"},
		{"bcs", "11000101"},
		{"bcc", "11000110"},
		{"bvs", "11000111"},
		{"bvc", "11001000"},

		// b4
		{"clc", "1110000000000000"},
		{"clv", "1110000000000001"},
		{"cls", "1110000000000010"},
		{"halt", "1110000000000011"},
		{"sec", "1110000000000100"},
		{"sev", "1110000000000101"},
		{"sez", "1110000000000110"},
		{"ses", "1110000000000111"},
		{"scc", "1110000000001000"},
		{"nop", "1110000000001001"},
		{"ret", "1110000000001010"},
		{"reti", "1110000000001011"},
		{"ccc", "1110000000001100"},
		{"wait", "1110000000001101"},
		{"pushpc", "1110000000001110"},
		{"poppc", "1110000000001110"},
		{"pushflag", "1110000000001111"}
	};
}

const string Transform::DIRECT_REGISTER = "00";
const string Transform::IMEDIATE_ADDRESS = "01";
const string Transform::INDIRECT_REGISTER = "10";
const string Transform::INDEX_ADDRESS = "11";

string Transform::GetInstructionBinaryCode(string opcode)
{
	return instructionDictionary[opcode];
}

// binary code of opcode
int Transform::GetInstructionType(string opcode)
{
	int type;

	if (opcode[0] == '0') type = 1;
	else if (opcode[1] == '0') type = 2;
	else if (opcode[2] == '0') type = 3;
	else type = 4;

	return type;
}

string Transform::IntToBinary(int value, short int operandLength)
{
	string result = "";

	while (value != 0)
	{
		result = (char)((value % 2) + '0') + result;
		value /= 2;
	}

	operandLength -= result.length();
	while (operandLength--)
	{
		result = "0" + result;
	}

	return result;
}

char Transform::BinaryLineToByte(string binary)
{
	char binaryString[17];
	strcpy_s(binaryString, 17, binary.c_str());
	int value = (int)strtol(binaryString, NULL, 2);

	return (char)value;
}

int Transform::StringToInt(string str, int startIndex, int endIndex)
{
	int tmp = 0;
	for (int i = startIndex; i < endIndex; i++)
	{
		tmp += (int)str[i] - '0';
		tmp *= 10;
	}

	return tmp / 10;
}

string Transform::OperandToBinary(string operand, short int length)
{
	string result;
	int indexR = -1, indexParantesesO = -1, indexParantasesC = -1;
	bool containR = false, containParanteses = false;

	for (int i = 0; i < operand.size(); i++) 
	{
		if (operand[i] == 'r')
		{
			indexR = i;
			containR = true;
		}
		else if (operand[i] == '(')
		{
			indexParantesesO = i;
			containParanteses = true;
		}
		else if (operand[i] == ')')
		{
			indexParantasesC = i;
		}
	}

	if (containR && containParanteses)
	{
		if (indexR > indexParantesesO && indexR < indexParantasesC)
		{
			result = INDIRECT_REGISTER + IntToBinary((int)operand[indexR + 1] - '0', length);
		} 
		else
		{
			int tmp = StringToInt(operand, indexParantesesO + 1, indexParantasesC);
			result += INDEX_ADDRESS + IntToBinary((int)operand[indexR + 1] - '0', length);
			result += IntToBinary(tmp, FULL_LEN);
		}
	}
	else if (containR && !containParanteses)
	{
		result = DIRECT_REGISTER + IntToBinary((int)operand[indexR + 1] - '0', length);
	}
	else
	{
		int tmp = StringToInt(operand, 0, operand.size());
		result = IMEDIATE_ADDRESS + IntToBinary(tmp, length);
	}

	return result;
}
