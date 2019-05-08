#pragma once
/*************************************************************
## BinaryWriter ##
@@ Author : ±Ç¼ø¿ì , Date : 2019.02.24
**************************************************************/
class BinaryWriter final
{
	BlockAssign(BinaryWriter)
public:
	BinaryWriter();
	~BinaryWriter();

	void Open(const wstring& filePath, UINT openOption = CREATE_ALWAYS);
	void Close();

	void Bool(const bool& data);
	void Word(const WORD& data);
	void Int(const int& data);
	void UInt(const UINT& data);
	void Float(const float& data);
	void Double(const double& data);
	void Vector2D(const Vector2& data);
	void String(const string& data);
	void Byte(void* data,const UINT& dataSize);
protected:
	HANDLE fileHandle;
	DWORD size;
};

//////////////////////////////////////////////////////////////////////////

class BinaryReader final
{
	BlockAssign(BinaryReader)
public:
	BinaryReader();
	~BinaryReader();

	bool Open(const wstring& filePath);
	void Close();

	bool Bool();
	WORD Word();
	int Int();
	UINT UInt();
	float Float();
	double Double();
	Vector2 Vector2D();
	string String();
	void Byte(void** data,const UINT& dataSize);

protected:
	HANDLE fileHandle;
	DWORD size;
};