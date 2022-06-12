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

		void cgShowRegisters();
		void cgRegisterContent(int registerName, Platform::String^ value);
		void cgTContent(Platform::String^ value);
		void cgPCContent(Platform::String^ value);
		void cgSPContent(Platform::String^ value);
		void cgFLAGSContent(Platform::String^ value);
		void cgBE1Content(Platform::String^ value);
		void cgBE0Content(Platform::String^ value);
		void cgBVIContent(Platform::String^ value);
		void cgBPOContent(Platform::String^ value);
		void cgMARContent(Platform::String^ value);
		void cgMIRContent(Platform::String^ value);
		void cgADRContent(Platform::String^ value);
		void cgMDRContent(Platform::String^ value);
		void cgIRContent(Platform::String^ value);
		void cgSBUSContent(Platform::String^ value);
		void cgDBUSContent(Platform::String^ value);
		void cgRBUSContent(Platform::String^ value);

		void cgZContent(Platform::String^ value);
		void cgCContent(Platform::String^ value);
		void cgNContent(Platform::String^ value);
		void cgVContent(Platform::String^ value);
	private:
		void Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void path_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
		void Button_Click_1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Button_Click_2(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void R1_Copy_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void MDR_content_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

	

	};
}
