#pragma once
/****************************************************************************
## StringHelper ##
@@ Author : ±Ç¼ø¿ì , Date : 2019.03.20
*****************************************************************************/
class StringHelper final
{
public:
	static void SplitString(vector<string>* result, string origin, string tok);
	static void SplitString(vector<wstring>* result, wstring origin, wstring tok);

	static bool StartsWith(string str, string comp);
	static bool StartsWith(wstring str, wstring comp);

	static bool Contain(string str, string comp);
	static bool Contain(wstring str, wstring comp);

	static void Replace(string* str, string comp, string rep);
	static void Replace(wstring* str, wstring comp, wstring rep);

	static wstring StringToWString(string value);
	static string WStringToString(wstring value);
};