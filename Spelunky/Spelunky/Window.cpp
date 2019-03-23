#include "stdafx.h"
#include "Window.h"

#include "Program.h"

/************************************************************************
## WindowDesc ##
************************************************************************/
Window::Desc Window::_windowDesc;

void Window::SetDesc(const Desc * const pDesc)
{
	_windowDesc = *pDesc;
}

void Window::GetDesc(Desc * const pDesc)
{
	*pDesc = _windowDesc;
}
/************************************************************************
## Window ##
������ ���� �� �������� ���
************************************************************************/
Window::Window()
{
	RECT rect = { 0, 0, (LONG)_windowDesc.width, (LONG)_windowDesc.height };

	UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - (UINT)_windowDesc.width) / 2;
	UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - (UINT)_windowDesc.height) / 2;
	//������ ���� �ʱ�ȭ 
	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;										//Ŭ���� ���� �޸� ����
	wndClass.cbWndExtra = 0;										//������ ���� �޸� ����
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	//������ ��׶���
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					//������ Ŀ�� ���
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//������ ������ ���
	wndClass.hInstance = _windowDesc.instance;						//������ �ν��Ͻ� ��ȣ
	wndClass.lpfnWndProc = (WNDPROC)Window::WndProc;				//������ ���ν���
	wndClass.lpszClassName = _windowDesc.appName.c_str();			//������ Ŭ���� �̸�
	wndClass.lpszMenuName = NULL;									//������ �޴� �̸�
	wndClass.style = CS_HREDRAW | CS_VREDRAW;						//������ â ��Ÿ��
	//�������� ���
	RegisterClass(&wndClass);
	//��ü ȭ���̶�� 
	if (_windowDesc.bFullScreen == true)
	{
		DEVMODE devMode = { 0 };
		devMode.dmSize = sizeof(DEVMODE);
		devMode.dmPelsWidth = (DWORD)_windowDesc.width;
		devMode.dmPelsHeight = (DWORD)_windowDesc.height;
		devMode.dmBitsPerPel = 32;
		devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
	}
	//������ ����
	_windowDesc.handle = CreateWindow
	(
		_windowDesc.appName.c_str(),
		_windowDesc.appName.c_str(),
		WS_OVERLAPPEDWINDOW,
		centerX - _windowDesc.width / 2,
		centerY - _windowDesc.height / 2,
		_windowDesc.width,
		_windowDesc.height,
		NULL,
		(HMENU)NULL,
		_windowDesc.instance,
		NULL
	);
	assert(_windowDesc.handle != NULL);
	//HDC 
	_windowDesc.hdc = GetDC(_windowDesc.handle);
	//WindowRect 
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	//WindowPos
	SetWindowPos(_windowDesc.handle, NULL, centerX - _windowDesc.width / 2, centerY - _windowDesc.height / 2, _windowDesc.width, _windowDesc.height
		, SWP_NOZORDER | SWP_NOMOVE);
	//MoveWindow
	MoveWindow
	(
		_windowDesc.handle
		, centerX, centerY
		, rect.right - rect.left, rect.bottom - rect.top
		, TRUE
	);
	//ShowWindow
	ShowWindow(_windowDesc.handle, Window::_windowDesc.command);
	ShowCursor(true);
}

/************************************************************************
## ~Window ##
������ ,�������� ��� ����
************************************************************************/
Window::~Window()
{
	if (_windowDesc.bFullScreen == true)
		ChangeDisplaySettings(NULL, 0);

	DestroyWindow(_windowDesc.handle);

	UnregisterClass(_windowDesc.appName.c_str(), _windowDesc.instance);
}
/************************************************************************
## WndProc ##
���� ���Ͻ�
************************************************************************/
LRESULT Window::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_MOUSEMOVE:
		Input::Get()->InputProc(message, wParam, lParam);
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostMessage(handle, WM_DESTROY, 0, 0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return (DefWindowProc(handle, message, wParam, lParam));
}


/*************************************************
## Run ##
���� ����
**************************************************/
WPARAM Window::Run()
{
	MSG msg = { 0 };
	//=========Init Manager=================================
	this->CreateSingleton();
	//=======================================================
	this->_program = new Program;
	this->_program->Init();
	TimeManager::Get()->Start();
	//============Main Loop===================================
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			this->UpdateSingleton();
			this->_program->Update();
			this->_program->Render();
		}
	}
	//=======================================================
	_program->Release();
	SafeDelete(_program);
	//================Release Manager===============================
	this->DeleteSingleton();
	//==============================================================

	return msg.wParam;
}



/*************************************************
## CreateSingleton ##
**************************************************/
void Window::CreateSingleton()
{
	TimeManager::Create();
	Input::Create();
	D3DRenderer::Create();
	D2DRenderer::Create();
	ImageManager::Create();
}
/*************************************************
## DeleteSingleton ##
**************************************************/
void Window::DeleteSingleton()
{
	ImageManager::Delete();
	D2DRenderer::Delete();
	D3DRenderer::Delete();
	Input::Delete();
	TimeManager::Delete();
}
/*************************************************
## UpdateSingleton ##
**************************************************/
void Window::UpdateSingleton()
{
	Input::Get()->Update();
	TimeManager::Get()->Update();
}