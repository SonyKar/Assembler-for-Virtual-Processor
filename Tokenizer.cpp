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
		size_t pos = inputText.find_first_of("\r\n");
		string line = inputText.substr(0, pos);
		if (line.find(",") != string::npos) {
			/// <summary>
			/// instruction with 2 oprerands(b1)
			/// </summary>
			/// <param name="sender"></param>
			/// <param name="e"></param>
			string opcode = line.substr(0, line.find_first_of(' '));
			size_t posOfComma = line.find(',');
			size_t posOfSpace = line.find_first_of(' ');
			string aux = line.substr(posOfSpace + 1);
			string first_operand = aux.substr(0, aux.find(','));
			string binary_first_operand = transform->OperandToBinary(first_operand, 4);
			string binary_first_operand_indexed = "";
			if (binary_first_operand.length() > 6) {
				binary_first_operand_indexed = binary_first_operand.substr(6, binary_first_operand.length());
				binary_first_operand = binary_first_operand.substr(0, 6);
			}
			string second_operand = line.substr(posOfComma + 1);
			string binary_second_operand = transform->OperandToBinary(second_operand, 4);
			string binary_second_operand_indexed = "";
			if (binary_second_operand.length() > 6) {
				binary_second_operand_indexed = binary_second_operand.substr(6, binary_second_operand.length());
				binary_second_operand = binary_second_operand.substr(0, 6);
			}
			outputText += transform->GetInstructionBinaryCode(opcode);
			outputText += binary_second_operand;
			outputText += binary_first_operand;
			outputText += binary_second_operand_indexed;
			outputText += binary_first_operand_indexed;
		}
		else {
			//string opcode = line.substr(0, line.find_first_of(' '));
			//string operand = 
		}
		inputText = inputText.substr(pos + 2, inputText.length());
	}
	return outputText;
}
