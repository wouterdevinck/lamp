#include "stdafx.h"
#include "MarshalHelper.h"

using namespace LampInterop;

string MarshalHelper::Convert(String^ from) {
	return marshal_as<string>(from);
}

String^ MarshalHelper::Convert(string from) {
	return gcnew String(from.c_str());
}

vector<uint8_t> MarshalHelper::Convert(List<Byte>^ from) {
	auto result = vector<uint8_t>();
	for each (Byte val in from) {
		result.push_back(val);
	}
	return result;
}

List<Byte>^ MarshalHelper::Convert(vector<uint8_t> from) {
	auto result = gcnew List<Byte>();
	for (uint8_t val : from) {
		result->Add(val);
	}
	return result;
}

map<string, string> MarshalHelper::Convert(Dictionary<String^, String^>^ from) {
	auto result = map<string, string>();
	for each (auto kvp in from) {
		auto key = Convert(kvp.Key);
		auto val = Convert(kvp.Value);
		result.insert(pair<string, string>(key, val));
	}
	return result;
}

Dictionary<String^, String^>^ MarshalHelper::Convert(map<string, string> from) {
	auto result = gcnew Dictionary<String^, String^>(from.size());
	for (auto kvp : from) {
		auto key = Convert(kvp.first.c_str());
		auto val = Convert(kvp.second.c_str());
		result->Add(key, val);
	}
	return result;
}