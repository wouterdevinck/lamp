#pragma once

#include <gcroot.h>
#include <msclr\marshal_cppstd.h>

#include <string>
#include <cstdint>
#include <vector>
#include <map>

using namespace System;
using namespace System::Collections::Generic;
using namespace msclr::interop;
using namespace std;

namespace LampInterop {

	class MarshalHelper {

	public:

		// Strings
		static string Convert(String^ from);
		static String^ Convert(string from);

		// Lists
		static vector<uint8_t> Convert(List<Byte>^ from);
		static List<Byte>^ Convert(vector<uint8_t> from);

		// Dictionaries
		static map<string, string> Convert(Dictionary<String^, String^>^ from);
		static Dictionary<String^, String^>^ Convert(map<string, string> from);

	};

}