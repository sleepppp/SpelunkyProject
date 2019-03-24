#pragma once

/*********************************************
## TextWriter ##
텍스트 파일 저장 용
**********************************************/
class TextWriter
{
private:
	static const char spaceToken;
	static const char endLineToken;
	static const char endFileToken;
private:
	FILE* file;
	string tempStr;
public:
	TextWriter() :file(nullptr){}

	bool Open(const string& filePath);
	void Close();

	void String(const string& str);
	void Int(const int& i);
	void Float(const float& f);
	void Bool(const bool& b);
	void UInt(const UINT& ui);

	void EndLine();
};
/*********************************************
## TextReader ##
텍스트 파일 읽기 용
**********************************************/
class TextReader
{
private:
	static const char spaceToken;
	static const char endLineToken;
	static const char endFileToken;
private:
	FILE* file;
	string tempStr;
	int endIndex;
public:
	TextReader() :file(nullptr) ,endIndex(0){}

	bool Open(const string& filePath);
	void Close();

	string String();
	int Int();
	float Float();
	bool Bool();
	UINT UInt();

	void ReadLine();
};