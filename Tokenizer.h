#pragma once
#include <sstream>
#include <Transform.h>
#include <Helper.h>

using namespace std;

namespace tokenizer
{
	class Tokenizer
	{
	public :
		static string tokenize(string inputText);
		
	};

	struct etiquette {
		string name;
		int pcPosition;
	};
}

