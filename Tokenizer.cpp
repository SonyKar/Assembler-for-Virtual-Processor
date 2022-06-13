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
	string inputTextCopy = inputText;
	
	int pcCounter = 0;
	list<etiquette> etiquettes = {};
	while (inputTextCopy.length() != 0) {
		size_t pos = inputTextCopy.find_first_of("\r\n");
		string line = inputTextCopy.substr(0, pos);
		if (line.find(':') != line.npos) {
			etiquette myEtiquette;
			myEtiquette.name = line.replace(line.find_first_of(':'), line.find_first_of(':') + 1, "");
			myEtiquette.pcPosition = pcCounter + 1;
			etiquettes.push_front(myEtiquette);
		}
		else 
		{
			switch (transform->GetInstructionType(transform->GetInstructionBinaryCode(line.substr(0, line.find_first_of(' ')))))
			{
				case 1:
				{
					size_t posOfComma = line.find(',');
					size_t posOfSpace = line.find_first_of(' ');
					string aux = line.substr(posOfSpace + 1);
					string destination_operand = aux.substr(0, aux.find(','));
					string binary_destination_operand = transform->OperandToBinary(destination_operand, 4);
					string source_operand = line.substr(posOfComma + 1);
					string binary_source_operand = transform->OperandToBinary(source_operand, 4);
					char addressType[3] = { binary_source_operand[0], binary_source_operand[1], '\0'};
					if (addressType == transform->IMEDIATE_ADDRESS || addressType == transform->INDEX_ADDRESS)
					{
						pcCounter++;
					}
					addressType[0] = binary_destination_operand[0];
					addressType[1] = binary_destination_operand[1];
					if (addressType == transform->IMEDIATE_ADDRESS || addressType == transform->INDEX_ADDRESS)
					{
						pcCounter++;
					}
					break;
				}
				case 2:
				{
					string destination_operand = line.substr(line.find(' ') + 1, line.length());
					string binary_destination_operand = transform->OperandToBinary(destination_operand, 4);
					char addressType[2] = { binary_destination_operand[0], binary_destination_operand[1] };
					if (addressType == transform->IMEDIATE_ADDRESS || addressType == transform->INDEX_ADDRESS)
					{
						pcCounter++;
					}
					break;
				}
				case 3:
				{
					pcCounter++;
					break;
				}
			}
		}
		inputTextCopy = inputTextCopy.substr(pos + 2, inputTextCopy.length());
		pcCounter++;
	}
	pcCounter = 0;
	while (inputText.length() != 0) {
		size_t pos = inputText.find_first_of("\r\n");
		string line = inputText.substr(0, pos);
		if (line.find(':') == line.npos) {
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
				char addressType[3] = { binary_source_operand[0], binary_source_operand[1], '\0' };
				if (addressType == transform->IMEDIATE_ADDRESS || addressType == transform->INDEX_ADDRESS)
				{
					pcCounter++;
				}
				addressType[0] = binary_destination_operand[0];
				addressType[1] = binary_destination_operand[1];
				if (addressType == transform->IMEDIATE_ADDRESS || addressType == transform->INDEX_ADDRESS)
				{
					pcCounter++;
				}
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
				char addressType[3] = { binary_destination_operand[0], binary_destination_operand[1], '\0' };
				if (addressType == transform->IMEDIATE_ADDRESS || addressType == transform->INDEX_ADDRESS)
				{
					pcCounter++;
				}
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
				string opcode_binary = transform->GetInstructionBinaryCode(line.substr(0, line.find(' ')));
				string operand = line.substr(line.find(' ') + 1, line.length());
				for (auto it = etiquettes.begin(); it != etiquettes.end(); it++) {
					if (it->name == operand)
					{
						int relativePosition = it->pcPosition - pcCounter;
						if (relativePosition < 0) relativePosition -= 2;
						else relativePosition -= 3;
						operand = transform->OperandToBinary(to_string(relativePosition), 4, true); // 00 + IM + 0000
					}
				}
				outputText += opcode_binary;
				outputText += operand;
				pcCounter++;
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
		}
		inputText = inputText.substr(pos + 2, inputText.length());
		pcCounter++;
	}
	return outputText;
}
