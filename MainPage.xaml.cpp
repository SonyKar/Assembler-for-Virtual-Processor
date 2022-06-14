//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include <stdio.h>
#include <ppltasks.h>
#include <string>
#include "pch.h"
#include "MainPage.xaml.h"
#include <Transform.h>
#include <sstream>
#include <Tokenizer.h>
#include <Helper.h>
#include <Processor.h>
#include <ProcessorStructure.h>
using namespace Assembler_for_Virtual_Processor;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Streams;
using namespace concurrency;
using namespace Windows::Devices::Enumeration;
using namespace tokenizer;
// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();
	cpuShowParameters();
}

void Assembler_for_Virtual_Processor::MainPage::Pick_File_Handler(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

	StorageFolder^ storageFolder = ApplicationData::Current->LocalFolder;
	concurrency::create_task(storageFolder->CreateFileAsync("output.bin", CreationCollisionOption::ReplaceExisting));

	assembler::Transform* transform = new assembler::Transform();
	FileOpenPicker^ fileOpenPicker = ref new FileOpenPicker();
	Array<String^>^ fileTypes = ref new Array<String^>(1);
	fileTypes->Data[0] = ".asm";
	fileOpenPicker->ViewMode = PickerViewMode::Thumbnail;
	fileOpenPicker->SuggestedStartLocation = PickerLocationId::Desktop;
	fileOpenPicker->FileTypeFilter->ReplaceAll(fileTypes);
	create_task(fileOpenPicker->PickSingleFileAsync()).then([this](StorageFile^ file)
		{
			if (file)
			{
				source_path->Text = file->Path;
				return FileIO::ReadBufferAsync(file);
			}
		}).then([this](Streams::IBuffer^ buffer) {

			auto dataReader = DataReader::FromBuffer(buffer);
			String^ bufferText = dataReader->ReadString(buffer->Length);
			content->Text = bufferText;
			}
		);
}

void Assembler_for_Virtual_Processor::MainPage::Generate_Binary_Output(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	assembler::Transform* transform = new assembler::Transform();
	string ouputTextBin = tokenizer::Tokenizer::tokenize(helper::Helper::platformStringToStdString(content->Text));
	output->Text = "";
	for (int i = 0; i < ouputTextBin.length() / 16; i++) {
		string line = ouputTextBin.substr(0, (1 + i) * 16);
		line = line.substr(i * 16, line.length()) + "\n";
		if (line[0] == '-') line[0] = '1';
		output->Text += helper::Helper::StdStringToPlatformString(line);
	}
	string ouputTextBinWriteTask = ouputTextBin;
	list<wchar_t> hexOutput;
	while (ouputTextBinWriteTask.length() >= 16)
	{
		string line = ouputTextBinWriteTask.substr(0, 16);
		wchar_t hexLineOutput = transform->BinaryLineToByte(line);
		hexOutput.push_back(hexLineOutput);
		ouputTextBinWriteTask = ouputTextBinWriteTask.substr(16, ouputTextBinWriteTask.length());
	}
	//moved to back end
	int memPositionStore = 32;
	for (auto it = hexOutput.begin(); it != hexOutput.end(); it++)
	{
		ProcessorStructure::MEM[memPositionStore++] = *it;
	}

	StorageFolder^ storageFolder = ApplicationData::Current->LocalFolder;
	destination_path->Text = ApplicationData::Current->LocalFolder->Path;

	create_task(storageFolder->GetFileAsync("output.bin")).then([hexOutput](StorageFile^ outputFile)
		{
			create_task(outputFile->OpenAsync(FileAccessMode::ReadWrite)).then([hexOutput, outputFile](IRandomAccessStream^ stream)
				{
					// Process stream
					IOutputStream^ outputStream = stream->GetOutputStreamAt(0);
					DataWriter^ dataWriter = ref new DataWriter(outputStream);
					for (auto it = hexOutput.begin(); it != hexOutput.end(); it++)
					{
						wchar_t atom = *it;
						dataWriter->WriteInt16(atom);
					}
					dataWriter->StoreAsync();
					outputStream->FlushAsync();
				});
		});


}

void Assembler_for_Virtual_Processor::MainPage::Run_Halt_Handler(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Processor::RunHalt();
	if (isHex) cpuShowParametersHex();
	else cpuShowParameters();
}

void Assembler_for_Virtual_Processor::MainPage::Run_Step_Handler(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Processor::RunStep();
	if (isHex) cpuShowParametersHex();
	else cpuShowParameters();
}

void Assembler_for_Virtual_Processor::MainPage::cpuShowParameters()
{
	cgTContent(helper::Helper::StdStringToPlatformString(std::to_string(ProcessorStructure::T)));
	cgPCContent(helper::Helper::StdStringToPlatformString(std::to_string(ProcessorStructure::PC)));
	cgSPContent(helper::Helper::StdStringToPlatformString(std::to_string(ProcessorStructure::SP)));
	cgFLAGSContent(helper::Helper::StdStringToPlatformString(std::to_string(ProcessorStructure::flags)));
	cgBE1Content(helper::Helper::StdStringToPlatformString(std::to_string(ProcessorStructure::BE1)));
	cgBE0Content(helper::Helper::StdStringToPlatformString(std::to_string(ProcessorStructure::BE0)));
	cgBVIContent(helper::Helper::StdStringToPlatformString(std::to_string(ProcessorStructure::BVI)));
	cgBPOContent(helper::Helper::StdStringToPlatformString(std::to_string(ProcessorStructure::BPO)));
	cgMARContent(helper::Helper::StdStringToPlatformString(std::to_string(ProcessorStructure::MAR)));
	cgMIRContent(helper::Helper::StdStringToPlatformString(std::to_string(ProcessorStructure::MIR)));
	cgMDRContent(helper::Helper::StdStringToPlatformString(std::to_string(ProcessorStructure::MDR)));
	cgADRContent(helper::Helper::StdStringToPlatformString(std::to_string(ProcessorStructure::ADR)));
	cgIRContent(helper::Helper::StdStringToPlatformString(std::to_string(ProcessorStructure::IR)));
	cgSBUSContent(helper::Helper::StdStringToPlatformString(std::to_string(ProcessorStructure::SBUS)));
	cgDBUSContent(helper::Helper::StdStringToPlatformString(std::to_string(ProcessorStructure::DBUS)));
	cgRBUSContent(helper::Helper::StdStringToPlatformString(std::to_string(ProcessorStructure::RBUS)));
	cgZContent(helper::Helper::StdStringToPlatformString(std::to_string(ProcessorStructure::Z)));
	cgCContent(helper::Helper::StdStringToPlatformString(std::to_string(ProcessorStructure::C)));
	cgNContent(helper::Helper::StdStringToPlatformString(std::to_string(ProcessorStructure::N)));
	cgVContent(helper::Helper::StdStringToPlatformString(std::to_string(ProcessorStructure::V)));
	for (int i = 0; i < 16; i++)
	{
		cgRegisterContent(i, helper::Helper::StdStringToPlatformString(std::to_string(ProcessorStructure::RG[i])));
	}
}

void Assembler_for_Virtual_Processor::MainPage::cpuShowParametersHex()
{
	assembler::Transform transform;
	cgTContent(helper::Helper::StdStringToPlatformString(transform.IntToHex(ProcessorStructure::T)));
	cgPCContent(helper::Helper::StdStringToPlatformString(transform.IntToHex(ProcessorStructure::PC)));
	cgSPContent(helper::Helper::StdStringToPlatformString(transform.IntToHex(ProcessorStructure::SP)));
	cgFLAGSContent(helper::Helper::StdStringToPlatformString(transform.IntToHex(ProcessorStructure::flags)));
	cgBE1Content(helper::Helper::StdStringToPlatformString(transform.IntToHex(ProcessorStructure::BE1)));
	cgBE0Content(helper::Helper::StdStringToPlatformString(transform.IntToHex(ProcessorStructure::BE0)));
	cgBVIContent(helper::Helper::StdStringToPlatformString(transform.IntToHex(ProcessorStructure::BVI)));
	cgBPOContent(helper::Helper::StdStringToPlatformString(transform.IntToHex(ProcessorStructure::BPO)));
	cgMARContent(helper::Helper::StdStringToPlatformString(transform.IntToHex(ProcessorStructure::MAR)));
	cgMIRContent(helper::Helper::StdStringToPlatformString(transform.IntToHex(ProcessorStructure::MIR)));
	cgMDRContent(helper::Helper::StdStringToPlatformString(transform.IntToHex(ProcessorStructure::MDR)));
	cgADRContent(helper::Helper::StdStringToPlatformString(transform.IntToHex(ProcessorStructure::ADR)));
	cgIRContent(helper::Helper::StdStringToPlatformString(transform.IntToHex(ProcessorStructure::IR)));
	cgSBUSContent(helper::Helper::StdStringToPlatformString(transform.IntToHex(ProcessorStructure::SBUS)));
	cgDBUSContent(helper::Helper::StdStringToPlatformString(transform.IntToHex(ProcessorStructure::DBUS)));
	cgRBUSContent(helper::Helper::StdStringToPlatformString(transform.IntToHex(ProcessorStructure::RBUS)));
	cgZContent(helper::Helper::StdStringToPlatformString(transform.IntToHex(ProcessorStructure::Z)));
	cgCContent(helper::Helper::StdStringToPlatformString(transform.IntToHex(ProcessorStructure::C)));
	cgNContent(helper::Helper::StdStringToPlatformString(transform.IntToHex(ProcessorStructure::N)));
	cgVContent(helper::Helper::StdStringToPlatformString(transform.IntToHex(ProcessorStructure::V)));
	for (int i = 0; i < 16; i++)
	{
		cgRegisterContent(i, helper::Helper::StdStringToPlatformString(transform.IntToHex(ProcessorStructure::RG[i])));
	}
}

void Assembler_for_Virtual_Processor::MainPage::Dec_Representation_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	cpuShowParameters();
	isHex = false;
}

void Assembler_for_Virtual_Processor::MainPage::Hex_Representation_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	cpuShowParametersHex();
	isHex = true;
}

void Assembler_for_Virtual_Processor::MainPage::cgTContent(Platform::String^ value)
{
	this->T_content->Text = value;
}

void Assembler_for_Virtual_Processor::MainPage::cgPCContent(Platform::String^ value)
{
	this->PC_content->Text = value;
}

void Assembler_for_Virtual_Processor::MainPage::cgSPContent(Platform::String^ value)
{
	this->SP_content->Text = value;
}

void Assembler_for_Virtual_Processor::MainPage::cgFLAGSContent(Platform::String^ value)
{
	this->FLAGS_content->Text = value;
}

void Assembler_for_Virtual_Processor::MainPage::cgBE1Content(Platform::String^ value)
{
	this->BE1_content->Text = value;
}

void Assembler_for_Virtual_Processor::MainPage::cgBE0Content(Platform::String^ value)
{
	this->BE0_content->Text = value;
}

void Assembler_for_Virtual_Processor::MainPage::cgBVIContent(Platform::String^ value)
{
	this->BVI_content->Text = value;
}

void Assembler_for_Virtual_Processor::MainPage::cgBPOContent(Platform::String^ value)
{
	this->BPO_content->Text = value;
}

void Assembler_for_Virtual_Processor::MainPage::cgMARContent(Platform::String^ value)
{
	this->MAR_content->Text = value;
}

void Assembler_for_Virtual_Processor::MainPage::cgMIRContent(Platform::String^ value)
{
	this->MIR_content->Text = value;
}

void Assembler_for_Virtual_Processor::MainPage::cgADRContent(Platform::String^ value)
{
	this->ADR_content->Text = value;
}

void Assembler_for_Virtual_Processor::MainPage::cgMDRContent(Platform::String^ value)
{
	this->MDR_content->Text = value;
}

void Assembler_for_Virtual_Processor::MainPage::cgIRContent(Platform::String^ value)
{
	this->IR_content->Text = value;
}

void Assembler_for_Virtual_Processor::MainPage::cgSBUSContent(Platform::String^ value)
{
	this->SBUS_content->Text = value;
}

void Assembler_for_Virtual_Processor::MainPage::cgDBUSContent(Platform::String^ value)
{
	this->DBUS_content->Text = value;
}

void Assembler_for_Virtual_Processor::MainPage::cgRBUSContent(Platform::String^ value)
{
	this->RBUS_content->Text = value;
}

void Assembler_for_Virtual_Processor::MainPage::cgZContent(Platform::String^ value)
{
	this->Z_content->Text = value;
}

void Assembler_for_Virtual_Processor::MainPage::cgCContent(Platform::String^ value)
{
	this->C_content->Text = value;
}

void Assembler_for_Virtual_Processor::MainPage::cgNContent(Platform::String^ value)
{
	this->N_content->Text = value;
}

void Assembler_for_Virtual_Processor::MainPage::cgVContent(Platform::String^ value)
{
	this->V_content->Text = value;
}

void Assembler_for_Virtual_Processor::MainPage::cgRegisterContent(int registerName, Platform::String^ value)
{

	switch (registerName)
	{
	case 0:
		this->R0_content->Text = value;
		break;
	case 1:
		this->R1_content->Text = value;
		break;
	case 2:
		this->R2_content->Text = value;
		break;
	case 3:
		this->R3_content->Text = value;
		break;
	case 4:
		this->R4_content->Text = value;
		break;
	case 5:
		this->R5_content->Text = value;
		break;
	case 6:
		this->R6_content->Text = value;
		break;
	case 7:
		R7_content->Text = value;
		break;
	case 8:
		this->R8_content->Text = value;
		break;
	case 9:
		this->R9_content->Text = value;
		break;
	case 10:
		this->R10_content->Text = value;
		break;
	case 11:
		this->R11_content->Text = value;
		break;
	case 12:
		this->R12_content->Text = value;
		break;
	case 13:
		this->R13_content->Text = value;
		break;
	case 14:
		this->R14_content->Text = value;
		break;
	case 15:
		this->R15_content->Text = value;
		break;
	default:
		break;
	}

}