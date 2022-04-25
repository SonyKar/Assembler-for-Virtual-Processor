#pragma once
#include <string>
#include <sstream>

using namespace std;

namespace helper{
class Helper
{
	public:
	static Platform::String^ StdStringToPlatformString(std::string str);
	static string platformStringToStdString(Platform::String^ input);
};
}

