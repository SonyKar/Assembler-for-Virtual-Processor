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

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();
}
Platform::String^ StdStringToPlatformString(std::string str)
{
	std::wstringstream wss;
	wss << str.c_str();
	return ref new Platform::String(wss.str().c_str());
}

void Assembler_for_Virtual_Processor::MainPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    //assembler::Transform* transform = new assembler::Transform();
	//content->Text= StdStringToPlatformString(transform->BinaryLineToHex(transform->IntToBinary(7, transform->FULL_LEN)));
	//delete transform;

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
	StorageFolder^ storageFolder = ApplicationData::Current->LocalFolder;
	destination_path->Text = ApplicationData::Current->LocalFolder->Path;
	concurrency::create_task(storageFolder->CreateFileAsync("output.bin", CreationCollisionOption::ReplaceExisting));
	create_task(storageFolder->GetFileAsync("output.bin")).then([](StorageFile^ inputFile)
		{
			string test1 = "101010101011z100101001010101011";
			for (auto it = test1.begin(); it < test1.end(); it++) {
				*it = (*it) - 48;
			}
			create_task(FileIO::WriteTextAsync(inputFile, StdStringToPlatformString(test1)));
		});
}


