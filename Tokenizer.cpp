#include "pch.h"
#include "Tokenizer.h"

using namespace std;
using namespace assembler;

string tokenizer::Tokenizer::tokenize(string input){
	string inputText = input;
	assembler::Transform* transform = new assembler::Transform();
	string outputText="";
	std::for_each(inputText.begin(), inputText.end(), [](char& c)
	{
		c = ::tolower(c);
	});
	while (inputText.length() != 0) {
		int pcRelativeCounter = 0;
		size_t pos = inputText.find_first_of("\r\n");
		string line = inputText.substr(0, pos);
		//string opcode = line.substr(0, line.find_first_of(' '))
		int test = transform->GetInstructionType(line.substr(0, line.find_first_of(' ')));
		switch (transform->GetInstructionType(transform->GetInstructionBinaryCode(line.substr(0, line.find_first_of(' ')))))
		{
		case 1:
		{
			string opcode = line.substr(0, line.find_first_of(' '));
			size_t posOfComma = line.find(',');
			size_t posOfSpace = line.find_first_of(' ');
			string aux = line.substr(posOfSpace + 1);
			string destination_operand = aux.substr(0, aux.find(','));
			string binary_destination_operand = transform->OperandToBinary(destination_operand, 4);
			string binary_destination_operand_indexed = "";
			if (binary_destination_operand.length() > 6) {
				binary_destination_operand_indexed = binary_destination_operand.substr(6, binary_destination_operand.length());
				binary_destination_operand = binary_destination_operand.substr(0, 6);
			}
			string source_operand = line.substr(posOfComma + 1);
			string binary_source_operand = transform->OperandToBinary(source_operand, 4);
			string binary_source_operand_indexed = "";
			if (binary_source_operand.length() > 6) {
				binary_source_operand_indexed = binary_source_operand.substr(6, binary_source_operand.length());
				binary_source_operand = binary_source_operand.substr(0, 6);
			}
			outputText += transform->GetInstructionBinaryCode(opcode);
			outputText += binary_source_operand;
			outputText += binary_destination_operand;
			outputText += binary_source_operand_indexed;
			outputText += binary_destination_operand_indexed;
			break;
		}
		case 2:
		{
			string opcode_binary = transform->GetInstructionBinaryCode(line.substr(0, line.find(' ')));
			string destination_operand = line.substr(line.find(' ') + 1, line.length());
			string binary_destination_operand = transform->OperandToBinary(destination_operand, 4);
			string binary_destination_operand_indexed = "";
			string binary_source_operand_indexed = "";
			if (binary_destination_operand.length() > 6) {
				binary_destination_operand_indexed = binary_destination_operand.substr(6, binary_destination_operand.length());
				binary_destination_operand = binary_destination_operand.substr(0, 6);
				binary_source_operand_indexed = "0000000000000000";

			}
			outputText += opcode_binary;
			outputText += binary_destination_operand;
			outputText += binary_source_operand_indexed;
			outputText += binary_destination_operand_indexed;

			break;
		}
		case 3:
		{

			break;
		}
		case 4:
		{
			outputText += transform->GetInstructionBinaryCode(line);
			break;
		}
		default:
			break;
		}
		
		inputText = inputText.substr(pos + 2, inputText.length());
		pcRelativeCounter++;

	}
	return outputText;
}
