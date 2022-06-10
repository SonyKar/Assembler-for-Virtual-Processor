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
}

void Assembler_for_Virtual_Processor::MainPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
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


void Assembler_for_Virtual_Processor::MainPage::Button_Click_1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	assembler::Transform* transform = new assembler::Transform();
	string ouputTextBin = tokenizer::Tokenizer::tokenize(helper::Helper::platformStringToStdString(content->Text));
	output->Text = "";
	for (int i = 0; i < ouputTextBin.length() / 16; i++) {
		string line = ouputTextBin.substr(0, (1 + i) * 16);
		line = line.substr(i * 16, line.length()) + "\n";
		output->Text += helper::Helper::StdStringToPlatformString(line);
	}
	string ouputTextBinWriteTask = ouputTextBin;
	string hexOutput = "";
	while (ouputTextBinWriteTask.length() >= 8)
	{
		string line = ouputTextBinWriteTask.substr(0, 8);
		char hexLineOutput = transform->BinaryLineToByte(line);
		hexOutput += hexLineOutput;
		ouputTextBinWriteTask = ouputTextBinWriteTask.substr(8, ouputTextBinWriteTask.length());
	}
	//moved to back end
	int memPositionStore = 32;
	int sizeOf = hexOutput.length();
	for (int i = 0; i < hexOutput.length(); i = i + 2)
	{
		char upperHalf = hexOutput.at(i);
		char lowerHalf = hexOutput.at(i + 1);
		int instruction = ((((int)upperHalf) & 255) << 8) + (((int)lowerHalf) & 255);
		ProcessorStructure::MEM[memPositionStore++] = instruction;
	}

	StorageFolder^ storageFolder = ApplicationData::Current->LocalFolder;
	destination_path->Text = ApplicationData::Current->LocalFolder->Path;

	create_task(storageFolder->GetFileAsync("output.bin")).then([hexOutput](StorageFile^ outputFile)
		{
			create_task(outputFile->OpenAsync(FileAccessMode::ReadWrite)).then([hexOutput, outputFile](IRandomAccessStream^ stream)
				{
					// Process stream
					string output = hexOutput;
					IOutputStream^ outputStream = stream->GetOutputStreamAt(0);
					DataWriter^ dataWriter = ref new DataWriter(outputStream);
					while (output.length() > 0)
					{
						char atom = output.at(0);
						dataWriter->WriteByte(atom);
						output = output.substr(1, output.length());
					}
					dataWriter->StoreAsync();
					outputStream->FlushAsync();
				});
		});


}




void Assembler_for_Virtual_Processor::MainPage::Button_Click_2(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Processor::Run();
}


void Assembler_for_Virtual_Processor::MainPage::R1_Copy_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}


void Assembler_for_Virtual_Processor::MainPage::MDR_content_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}

void Assembler_for_Virtual_Processor::MainPage::cgRegisterContent(int registerName, std::string value)
{
	switch (registerName)
	{
	case 0:
		this->R0_content->Text = helper::Helper::StdStringToPlatformString(value);
		break;
	case 1:
		this->R1_content->Text = helper::Helper::StdStringToPlatformString(value);
		break;
	case 2:
		this->R2_content->Text = helper::Helper::StdStringToPlatformString(value);
		break;
	case 3:
		this->R3_content->Text = helper::Helper::StdStringToPlatformString(value);
		break;
	case 4:
		this->R4_content->Text = helper::Helper::StdStringToPlatformString(value);
		break;
	case 5:
		this->R5_content->Text = helper::Helper::StdStringToPlatformString(value);
		break;
	case 6:
		this->R6_content->Text = helper::Helper::StdStringToPlatformString(value);
		break;
	case 7:
		this->R7_content->Text = helper::Helper::StdStringToPlatformString(value);
		break;
	case 8:
		this->R8_content->Text = helper::Helper::StdStringToPlatformString(value);
		break;
	case 9:
		this->R9_content->Text = helper::Helper::StdStringToPlatformString(value);
		break;
	case 10:
		this->R10_content->Text = helper::Helper::StdStringToPlatformString(value);
		break;
	case 11:
		this->R11_content->Text = helper::Helper::StdStringToPlatformString(value);
		break;
	case 12:
		this->R12_content->Text = helper::Helper::StdStringToPlatformString(value);
		break;
	case 13:
		this->R13_content->Text = helper::Helper::StdStringToPlatformString(value);
		break;
	case 14:
		this->R14_content->Text = helper::Helper::StdStringToPlatformString(value);
		break;
	case 15:
		this->R15_content->Text = helper::Helper::StdStringToPlatformString(value);
		break;
	default:
		break;
	}

}

void Assembler_for_Virtual_Processor::MainPage::cgTContent(std::string value)
{
	this->T_content->Text = helper::Helper::StdStringToPlatformString(value);
}

void Assembler_for_Virtual_Processor::MainPage::cgPCContent(std::string value)
{
	this->PC_content->Text = helper::Helper::StdStringToPlatformString(value);
}

void Assembler_for_Virtual_Processor::MainPage::cgSPContent(std::string value)
{
	this->SP_content->Text = helper::Helper::StdStringToPlatformString(value);
}

void Assembler_for_Virtual_Processor::MainPage::cgFLAGSContent(std::string value)
{
	this->FLAGS_content->Text = helper::Helper::StdStringToPlatformString(value);
}

void Assembler_for_Virtual_Processor::MainPage::cgBE1Content(std::string value)
{
	this->BE1_content->Text = helper::Helper::StdStringToPlatformString(value);
}

void Assembler_for_Virtual_Processor::MainPage::cgBE0Content(std::string value)
{
	this->BE0_content->Text = helper::Helper::StdStringToPlatformString(value);
}

void Assembler_for_Virtual_Processor::MainPage::cgBVIContent(std::string value)
{
	this->BVI_content->Text = helper::Helper::StdStringToPlatformString(value);
}

void Assembler_for_Virtual_Processor::MainPage::cgBPOContent(std::string value)
{
	this->BPO_content->Text = helper::Helper::StdStringToPlatformString(value);
}

void Assembler_for_Virtual_Processor::MainPage::cgMARContent(std::string value)
{
	this->MAR_content->Text = helper::Helper::StdStringToPlatformString(value);
}

void Assembler_for_Virtual_Processor::MainPage::cgMIRContent(std::string value)
{
	this->MIR_content->Text = helper::Helper::StdStringToPlatformString(value);
}

void Assembler_for_Virtual_Processor::MainPage::cgADRContent(std::string value)
{
	this->ADR_content->Text = helper::Helper::StdStringToPlatformString(value);
}

void Assembler_for_Virtual_Processor::MainPage::cgMDRContent(std::string value)
{
	this->MDR_content->Text = helper::Helper::StdStringToPlatformString(value);
}

void Assembler_for_Virtual_Processor::MainPage::cgIRContent(std::string value)
{
	this->IR_content->Text = helper::Helper::StdStringToPlatformString(value);
}

void Assembler_for_Virtual_Processor::MainPage::cgSBUSContent(std::string value)
{
	this->SBUS_content->Text = helper::Helper::StdStringToPlatformString(value);
}

void Assembler_for_Virtual_Processor::MainPage::cgDBUSContent(std::string value)
{
	this->DBUS_content->Text = helper::Helper::StdStringToPlatformString(value);
}

void Assembler_for_Virtual_Processor::MainPage::cgRBUSContent(std::string value)
{
	this->RBUS_content->Text = helper::Helper::StdStringToPlatformString(value);
}

void Assembler_for_Virtual_Processor::MainPage::cgZContent(std::string value)
{
	this->C_content->Text = helper::Helper::StdStringToPlatformString(value);
}

void Assembler_for_Virtual_Processor::MainPage::cgCContent(std::string value)
{
	this->C_content->Text = helper::Helper::StdStringToPlatformString(value);
}

void Assembler_for_Virtual_Processor::MainPage::cgNContent(std::string value)
{
	this->N_content->Text = helper::Helper::StdStringToPlatformString(value);
}

void Assembler_for_Virtual_Processor::MainPage::cgOContent(std::string value)
{
	this->V_content->Text = helper::Helper::StdStringToPlatformString(value);
}

