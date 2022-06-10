//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"

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
		void cgRegisterContent(int registerName,std::string value);

		void cgTContent(string value);
		void cgPCContent(string value);
		void cgSPContent(string value);
		void cgFLAGSContent(string value);
		void cgBE1Content(string value);
		void cgBE0Content(string value);
		void cgBVIContent(string value);
		void cgBPOContent(string value);
		void cgMARContent(string value);
		void cgMIRContent(string value);
		void cgADRContent(string value);
		void cgMDRContent(string value);
		void cgIRContent(string value);
		void cgSBUSContent(string value);
		void cgBUSDContent(string value);
		void cgRBUSContent(string value);
		
		void cgZContent(string value);
		void cgCContent(string value);
		void cgNContent(string value);
		void cgOContent(string value);

	};
}
