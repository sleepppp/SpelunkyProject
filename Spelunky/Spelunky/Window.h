#pragma once
/****************************************************************************
## Window ##
@@ Author : �Ǽ��� , Date : 2019.03.20
*****************************************************************************/
class Window final
{
public:
	struct Desc
	{
		wstring appName;
		HINSTANCE instance;
		HWND handle;
		HDC hdc;
		int width;
		int height;
		int command;
		bool bFullScreen;
		bool bDirectX11; 
	};
private:
	static Desc _windowDesc;
public:
	static void SetDesc(const Desc*const pDesc);
	static void GetDesc(Desc*const pDesc);
	static int GetWidth() { return _windowDesc.width; }
	static int GetHeight() { return _windowDesc.height; }
	static const HWND GetHandle() { return _windowDesc.handle; }
	static const HDC GetHDC() { return _windowDesc.hdc; }
private:
	class Program* _program;
public:
	Window();
	virtual ~Window();

	WPARAM Run();
	static LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
private:
	void CreateSingleton();
	void DeleteSingleton();
	void UpdateSingleton();
};

#define _WinSizeX Window::GetWidth()
#define _WinSizeY Window::GetHeight()