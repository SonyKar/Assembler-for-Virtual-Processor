//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"
#include <Helper.cpp>

namespace Assembler_for_Virtual_Processor
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();

	private:
		void Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void path_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
		void Button_Click_1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Button_Click_2(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void R1_Copy_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void MDR_content_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	public:
		void cgRegisterContent(int registerName, std::string value);

		void cgTContent(std::string  value);
		void cgPCContent(std::string  value);
		void cgSPContent(std::string  value);
		void cgFLAGSContent(std::string  value);
		void cgBE1Content(std::string value);
		void cgBE0Content(std::string value);
		void cgBVIContent(std::string value);
		void cgBPOContent(std::string value);
		void cgMARContent(std::string value);
		void cgMIRContent(std::string value);
		void cgADRContent(std::string value);
		void cgMDRContent(std::string value);
		void cgIRContent(std::string value);
		void cgSBUSContent(std::string value);
		void cgDBUSContent(std::string value);
		void cgRBUSContent(std::string value);

		void cgZContent(std::string value);
		void cgCContent(std::string value);
		void cgNContent(std::string value);
		void cgOContent(std::string value);

	};
}
