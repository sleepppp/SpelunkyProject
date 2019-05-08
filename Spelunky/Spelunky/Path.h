#pragma once
/*************************************************************
## Path ##
@@ Author : ±Ç¼ø¿ì , Date : 2019.02.03
**************************************************************/
#include <functional>
#include <commdlg.h>
class Path
{
public:
	static bool ExistFile(string path);
	static bool ExistFile(wstring path);

	static bool ExistDirectory(wstring path);

	static string Combine(string path1, string path2);
	static wstring Combine(wstring path1, wstring path2);

	static string Combine(vector<string> paths);
	static wstring Combine(vector<wstring> paths);

	static string GetDirectoryName(string path);
	static wstring GetDirectoryName(wstring path);

	static string GetExtension(string path);
	static wstring GetExtension(wstring path);

	static string GetFileName(string path);
	static wstring GetFileName(wstring path);

	static string GetFileNameWithoutExtension(string path);
	static wstring GetFileNameWithoutExtension(wstring path);

	static string GetFilePathWithoutExtension(string path);
	static wstring GetFilePathWithoutExtension(wstring path);

	static void OpenFileDialog(wstring file, const WCHAR* filter, wstring folder, function<void(wstring)> func, HWND hwnd = NULL);
	static void SaveFileDialog(wstring file, const WCHAR* filter, wstring folder, function<void(wstring)> func, HWND hwnd = NULL);

	const static WCHAR* ImageFilter;
	const static WCHAR* BinModelFilter;
	const static WCHAR* FbxModelFilter;
	const static WCHAR* ShaderFilter;
	const static WCHAR* MaterialFilter;
	const static WCHAR* MeshFilter;
	const static WCHAR* AnimationFilter;
	const static WCHAR* ColliderFilter;
};