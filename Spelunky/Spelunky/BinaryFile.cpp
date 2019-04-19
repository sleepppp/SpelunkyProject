#include "stdafx.h"
#include "BinaryFile.h"

//////////////////////////////////////////////////////////////////////////

BinaryWriter::BinaryWriter()
	: fileHandle(NULL), size(0)
{

}

BinaryWriter::~BinaryWriter()
{

}

void BinaryWriter::Open(const wstring& filePath, UINT openOption)
{
	assert(filePath.length() > 0);
	fileHandle = CreateFile
	(
		filePath.c_str()
		, GENERIC_WRITE
		, 0
		, NULL
		, openOption
		, FILE_ATTRIBUTE_NORMAL
		, NULL
	);


	bool isChecked = fileHandle != INVALID_HANDLE_VALUE;
	assert(isChecked);
}

void BinaryWriter::Close()
{
	if (fileHandle != NULL)
	{
		CloseHandle(fileHandle);
		fileHandle = NULL;
	}
}

void BinaryWriter::Bool(const bool& data)
{
	WriteFile(fileHandle, &data, sizeof(bool), &size, NULL);
}

void BinaryWriter::Word(const WORD& data)
{
	WriteFile(fileHandle, &data, sizeof(WORD), &size, NULL);
}

void BinaryWriter::Int(const int& data)
{
	WriteFile(fileHandle, &data, sizeof(int), &size, NULL);
}

void BinaryWriter::UInt(const UINT& data)
{
	WriteFile(fileHandle, &data, sizeof(UINT), &size, NULL);
}

void BinaryWriter::Float(const float& data)
{
	WriteFile(fileHandle, &data, sizeof(float), &size, NULL);
}

void BinaryWriter::Double(const double& data)
{
	WriteFile(fileHandle, &data, sizeof(double), &size, NULL);
}

void BinaryWriter::Vector2D(const Vector2& data)
{
	WriteFile(fileHandle, &data, sizeof(Vector2), &size, NULL);
}


void BinaryWriter::String(const string & data)
{
	UInt(data.size());

	const char* str = data.c_str();
	WriteFile(fileHandle, str, data.size(), &size, NULL);
}

void BinaryWriter::Byte(void * data,const UINT& dataSize)
{
	WriteFile(fileHandle, data, dataSize, &size, NULL);
}

//////////////////////////////////////////////////////////////////////////

BinaryReader::BinaryReader()
	: fileHandle(NULL), size(0)
{

}

BinaryReader::~BinaryReader()
{

}

bool BinaryReader::Open(const wstring& filePath)
{
	assert(filePath.length() > 0);
	fileHandle = CreateFile
	(
		filePath.c_str()
		, GENERIC_READ
		, FILE_SHARE_READ
		, NULL
		, OPEN_EXISTING
		, FILE_ATTRIBUTE_NORMAL
		, NULL
	);


	bool isChecked = fileHandle != INVALID_HANDLE_VALUE;
	Assert(isChecked);
	return isChecked;
}

void BinaryReader::Close()
{
	if (fileHandle != NULL)
	{
		CloseHandle(fileHandle);
		fileHandle = NULL;
	}
}

bool BinaryReader::Bool()
{
	bool temp = false;
	ReadFile(fileHandle, &temp, sizeof(bool), &size, NULL);

	return temp;
}

WORD BinaryReader::Word()
{
	WORD temp = 0;
	ReadFile(fileHandle, &temp, sizeof(WORD), &size, NULL);

	return temp;
}

int BinaryReader::Int()
{
	int temp = 0;
	ReadFile(fileHandle, &temp, sizeof(int), &size, NULL);

	return temp;
}

UINT BinaryReader::UInt()
{
	UINT temp = 0;
	ReadFile(fileHandle, &temp, sizeof(UINT), &size, NULL);

	return temp;
}

float BinaryReader::Float()
{
	float temp = 0.0f;
	ReadFile(fileHandle, &temp, sizeof(float), &size, NULL);

	return temp;
}

double BinaryReader::Double()
{
	double temp = 0.0f;
	ReadFile(fileHandle, &temp, sizeof(double), &size, NULL);

	return temp;
}

Vector2 BinaryReader::Vector2D()
{
	Vector2 temp;
	ReadFile(fileHandle, &temp, sizeof(Vector2), &size, NULL);
	return temp;
}



string BinaryReader::String()
{
	UINT size = Int();

	char* temp = new char[size + 1];
	ReadFile(fileHandle, temp, sizeof(char) * size, &this->size, NULL); //데이터 읽기
	temp[size] = '\0';

	return temp;
}

void BinaryReader::Byte(void ** data,const UINT& dataSize)
{
	ReadFile(fileHandle, *data, dataSize, &size, NULL);
}
