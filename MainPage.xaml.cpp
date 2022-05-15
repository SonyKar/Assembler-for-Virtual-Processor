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
		line = line.substr(i*16, line.length()) + "\n";
		output->Text += helper::Helper::StdStringToPlatformString(line);
	}
	StorageFolder^ storageFolder = ApplicationData::Current->LocalFolder;
	destination_path->Text = ApplicationData::Current->LocalFolder->Path;
	concurrency::create_task(storageFolder->CreateFileAsync("output.bin", CreationCollisionOption::ReplaceExisting));
	create_task(storageFolder->GetFileAsync("output.bin")).then([ouputTextBin,transform](StorageFile^ inputFile)
	{
		string ouputTextBinWriteTask = ouputTextBin;
		string hexOutput = "";
		while (ouputTextBinWriteTask.length() >= 16)
		{
			char hexLineOutput = transform->BinaryLineToByte(ouputTextBinWriteTask.substr(0, 16));
			hexOutput += hexLineOutput;
			ouputTextBinWriteTask = ouputTextBinWriteTask.substr(16, ouputTextBinWriteTask.length());
		}
		create_task(FileIO::WriteTextAsync(inputFile, helper::Helper::StdStringToPlatformString(hexOutput)));	
	});
}


