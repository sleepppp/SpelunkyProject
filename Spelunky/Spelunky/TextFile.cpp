#include "stdafx.h"
#include "TextFile.h"

const char TextWriter::spaceToken = ',';
const char TextWriter::endLineToken = '\n';
const char TextWriter::endFileToken = '/';

/******************************************************
## Open ##
해당 경로를 통해 텍스트 파일을 생성한다. 

@@ string filePath : 파일 경로 
*******************************************************/
bool TextWriter::Open(const string& filePath)
{
	//fopen_s : c스타일의 파일입출력 
	//FILE*, 파일경로,읽는 방식
	//"w+t" 텍스트파일의 쓰기용으로 읽겠다. 
	//반환형은 0또는 그 이외의 값인데 
	//0을 반환한다면 파일을 불러오는데 성공했다는 뜻이다. 
	int succeeded = fopen_s(&this->file, filePath.c_str(), "w+t");
	//0이 아니라면 파일을 불러오는데 실패 했다는 뜻이므로 false반환
	if (succeeded != 0)
		return false;
	//여기까지 왔다면 성공했다는 뜻 
	return true;
}
/******************************************************
## Close ##
파일의 끝에 슬래쉬를 표시하면서 Open했던 파일을 닫는다.
*******************************************************/
void TextWriter::Close()
{
	if (file)
	{
		//파일의 끝에 슬래쉬를 적는다
		fwrite(&TextWriter::endFileToken, sizeof TextWriter::endFileToken, 1, file);
		//파일이 있다면 파일 닫는다.
		fclose(file);
	}
}
/******************************************************
## String ##
@@ string str : 저장할 문자열
*******************************************************/
void TextWriter::String(const string& str)
{
	tempStr += str + TextWriter::spaceToken;
}
/******************************************************
## Int ##
@@ int i : 저장할 int값 
*******************************************************/
void TextWriter::Int(const int& i)
{
	tempStr += to_string(i) + TextWriter::spaceToken;
}
/******************************************************
## Float ##
@@ float f : 저장할 float값
*******************************************************/
void TextWriter::Float(const float& f)
{
	tempStr += to_string(f) + TextWriter::spaceToken;
}
/******************************************************
## Bool ## 
@@ bool b : 저장할 불값
*******************************************************/
void TextWriter::Bool(const bool& b)
{
	tempStr += to_string(b) + TextWriter::spaceToken;
}
/******************************************************
## UInt ##
@@ UINT ui : 저장할 UINT값
*******************************************************/
void TextWriter::UInt(const UINT& ui)
{
	tempStr += to_string(ui) + TextWriter::spaceToken;
}
/******************************************************
## EndLine ##
한줄 띄워서 글을 쓰고 싶을 때 사용
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
해당 파일 경로의 텍스트 파일을 읽어온다. 

@@ string filePath : 파일 경로 
*******************************************************/
bool TextReader::Open(const string& filePath)
{
	bool succeeded = fopen_s(&file, filePath.c_str(), "r");
	if (succeeded != 0)
		return false;
	//첫줄은 기본적으로 읽어온다. 
	this->ReadLine();

	return true;
}
/******************************************************
## Close ##
파일을 닫아준다. 
*******************************************************/
void TextReader::Close()
{
	if (file)
		fclose(file);
}
/******************************************************
## String ##
@@ return String : 문자열반환
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
@@ return int : int값 반환
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
@@ return float : float값 반환
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
@@ return bool : 불값 반환
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
@@ return UINT : UINT값 반환
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
한줄 띄워서 읽을 때 사용
*******************************************************/
void TextReader::ReadLine()
{
	char str[256];
	fscanf_s(file, "%s", &str, sizeof str);
	this->tempStr = str;
	this->endIndex = 0;
}
