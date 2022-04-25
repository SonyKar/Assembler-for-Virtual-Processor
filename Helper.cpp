#include "pch.h"
#include "Helper.h"

Platform::String^ helper::Helper::StdStringToPlatformString(std::string str)
{
	std::wstringstream wss;
	wss << str.c_str();
	return ref new Platform::String(wss.str().c_str());
}
string helper::Helper::platformStringToStdString(Platform::String^ input) {
	std::wstring fooW(input->Begin());
	std::string output(fooW.begin(), fooW.end());
	return output;
}