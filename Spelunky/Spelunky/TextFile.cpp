#include "stdafx.h"
#include "TextFile.h"

const char TextWriter::spaceToken = ',';
const char TextWriter::endLineToken = '\n';
const char TextWriter::endFileToken = '/';

/******************************************************
## Open ##
�ش� ��θ� ���� �ؽ�Ʈ ������ �����Ѵ�. 

@@ string filePath : ���� ��� 
*******************************************************/
bool TextWriter::Open(const string& filePath)
{
	//fopen_s : c��Ÿ���� ��������� 
	//FILE*, ���ϰ��,�д� ���
	//"w+t" �ؽ�Ʈ������ ��������� �аڴ�. 
	//��ȯ���� 0�Ǵ� �� �̿��� ���ε� 
	//0�� ��ȯ�Ѵٸ� ������ �ҷ����µ� �����ߴٴ� ���̴�. 
	int succeeded = fopen_s(&this->file, filePath.c_str(), "w+t");
	//0�� �ƴ϶�� ������ �ҷ����µ� ���� �ߴٴ� ���̹Ƿ� false��ȯ
	if (succeeded != 0)
		return false;
	//������� �Դٸ� �����ߴٴ� �� 
	return true;
}
/******************************************************
## Close ##
������ ���� �������� ǥ���ϸ鼭 Open�ߴ� ������ �ݴ´�.
*******************************************************/
void TextWriter::Close()
{
	if (file)
	{
		//������ ���� �������� ���´�
		fwrite(&TextWriter::endFileToken, sizeof TextWriter::endFileToken, 1, file);
		//������ �ִٸ� ���� �ݴ´�.
		fclose(file);
	}
}
/******************************************************
## String ##
@@ string str : ������ ���ڿ�
*******************************************************/
void TextWriter::String(const string& str)
{
	tempStr += str + TextWriter::spaceToken;
}
/******************************************************
## Int ##
@@ int i : ������ int�� 
*******************************************************/
void TextWriter::Int(const int& i)
{
	tempStr += to_string(i) + TextWriter::spaceToken;
}
/******************************************************
## Float ##
@@ float f : ������ float��
*******************************************************/
void TextWriter::Float(const float& f)
{
	tempStr += to_string(f) + TextWriter::spaceToken;
}
/******************************************************
## Bool ## 
@@ bool b : ������ �Ұ�
*******************************************************/
void TextWriter::Bool(const bool& b)
{
	tempStr += to_string(b) + TextWriter::spaceToken;
}
/******************************************************
## UInt ##
@@ UINT ui : ������ UINT��
*******************************************************/
void TextWriter::UInt(const UINT& ui)
{
	tempStr += to_string(ui) + TextWriter::spaceToken;
}
/******************************************************
## EndLine ##
���� ����� ���� ���� ���� �� ���
*******************************************************/
void TextWriter::EndLine()
{
	tempStr += TextWriter::endLineToken;
	fwrite(tempStr.c_str(), tempStr.size(), 1, file);
	tempStr.clear();
}


/////////////////////////////////////////////////////////////////////////////////


const char TextReader::spaceToken = ',';
const char TextReader::endLineToken = '\n';
const char TextReader::endFileToken = '/';
/******************************************************
## Open ##
�ش� ���� ����� �ؽ�Ʈ ������ �о�´�. 

@@ string filePath : ���� ��� 
*******************************************************/
bool TextReader::Open(const string& filePath)
{
	bool succeeded = fopen_s(&file, filePath.c_str(), "r");
	if (succeeded != 0)
		return false;
	//ù���� �⺻������ �о�´�. 
	this->ReadLine();

	return true;
}
/******************************************************
## Close ##
������ �ݾ��ش�. 
*******************************************************/
void TextReader::Close()
{
	if (file)
		fclose(file);
}
/******************************************************
## String ##
@@ return String : ���ڿ���ȯ
*******************************************************/
string TextReader::String()
{
	endIndex = tempStr.find(TextReader::spaceToken);
	string returnValue = tempStr.substr(0, endIndex).c_str();
	tempStr.erase(0, endIndex + 1);

	return returnValue;
}
/******************************************************
## Int ##
@@ return int : int�� ��ȯ
*******************************************************/
int TextReader::Int()
{
	endIndex = tempStr.find(TextReader::spaceToken);
	int returnValue = atoi(tempStr.substr(0, endIndex).c_str());
	tempStr.erase(0, endIndex + 1);

	return returnValue;
}
/******************************************************
## Float ##
@@ return float : float�� ��ȯ
*******************************************************/
float TextReader::Float()
{
	endIndex = tempStr.find(TextReader::spaceToken);
	float returnValue = (float)atof(tempStr.substr(0, endIndex).c_str());
	tempStr.erase(0, endIndex + 1);

	return returnValue;
}
/******************************************************
## Bool ##
@@ return bool : �Ұ� ��ȯ
*******************************************************/
bool TextReader::Bool()
{
	endIndex = tempStr.find(TextReader::spaceToken);
	bool returnValue = (int)atoi(tempStr.substr(0, endIndex).c_str());
	tempStr.erase(0, endIndex + 1);
	return returnValue;
}
/******************************************************
## UInt ##
@@ return UINT : UINT�� ��ȯ
*******************************************************/
UINT TextReader::UInt()
{
	endIndex = tempStr.find(TextReader::spaceToken);
	UINT returnValue = (UINT)atoi(tempStr.substr(0, endIndex).c_str());
	tempStr.erase(0, endIndex + 1);
	return returnValue;
}

/******************************************************
## ReadLine ##
���� ����� ���� �� ���
*******************************************************/
void TextReader::ReadLine()
{
	char str[256];
	fscanf_s(file, "%s", &str, sizeof str);
	this->tempStr = str;
	this->endIndex = 0;
}
