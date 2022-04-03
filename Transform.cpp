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
		{"ccc", "1110000000000011"},
		{"sec", "1110000000000100"},
		{"sev", "1110000000000101"},
		{"sez", "1110000000000110"},
		{"ses", "1110000000000111"},
		{"scc", "1110000000001000"},
		{"nop", "1110000000001001"},
		{"ret", "1110000000001010"},
		{"reti", "1110000000001011"},
		{"halt", "1110000000001100"},
		{"wait", "1110000000001101"},
		{"pushpc", "1110000000001110"},
		{"poppc", "1110000000001110"},
		{"pushflag", "1110000000001111"}
	};
}

string Transform::GetInstructionBinaryCode(string instruction)
{
	return instructionDictionary[instruction];
}

string Transform::IntToBinary(int value, short int operandLength)
{
	string result = "";

	while (value != 0)
	{
		result += ((value % 2) + '0');
		value /= 2;
	}

	operandLength -= result.length();
	while (operandLength--)
	{
		result = "0" + result;
	}

	return result;
}

string assembler::Transform::BinaryLineToHex(string binary)
{
	char binaryString[17];
	strcpy_s(binaryString, 17, binary.c_str());

	int value = (int)strtol(binaryString, NULL, 2);
	char hexString[3];
	sprintf_s(hexString, 3, "%x", value);

	return hexString;
}

