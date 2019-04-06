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
윈도우 생성 및 레지스터 등록
************************************************************************/
Window::Window()
{
	RECT rect = { 0, 0, (LONG)_windowDesc.width, (LONG)_windowDesc.height };

	UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - (UINT)_windowDesc.width) / 2;
	UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - (UINT)_windowDesc.height) / 2;
	//윈도우 정보 초기화 
	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;										//클래스 여분 메모리 설정
	wndClass.cbWndExtra = 0;										//윈도우 여분 메모리 설정
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	//윈도우 백그라운드
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					//윈도우 커서 모양
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//윈도우 아이콘 모양
	wndClass.hInstance = _windowDesc.instance;						//윈도우 인스턴스 번호
	wndClass.lpfnWndProc = (WNDPROC)Window::WndProc;				//윈도우 프로시져
	wndClass.lpszClassName = _windowDesc.appName.c_str();			//윈도우 클래스 이름
	wndClass.lpszMenuName = NULL;									//윈도우 메뉴 이름
	wndClass.style = CS_HREDRAW | CS_VREDRAW;						//윈도우 창 스타일
	//레지스터 등록
	RegisterClass(&wndClass);
	//전체 화면이라면 
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
	//윈도우 생성
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
	ShowWindow(_windowDesc.handle, Window::_windowDesc.command);
	ShowCursor(true);
}

/************************************************************************
## ~Window ##
윈도우 ,레지스터 등록 해제
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
메인 프록시
************************************************************************/
LRESULT Window::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui::WndProc((UINT*)handle, message, wParam, lParam)) return true;
	switch (message)
	{
	case WM_MOUSEMOVE:
		Input::Get()->InputProc(message, wParam, lParam);
		break;
	case WM_MOUSEWHEEL :
		CameraManager::Get()->CameraProc(wParam);
		break;
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
메인 루프
**************************************************/
WPARAM Window::Run()
{
	MSG msg = { 0 };
	//=========Init Manager=================================
	this->CreateSingleton();
	//=======================================================
	this->_program = new Program;
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
			CameraManager::Get()->Update();
			this->_program->Render();
		}
	}
	//=======================================================
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
	CameraManager::Create();
	D3DRenderer::Create();
	D2DRenderer::Create();
	ImGui::Create(Window::GetHandle(), _D3DDevice, _D3DDeviceContext);
	{
		ImGui::StyleColorsClassic();
	}
	ImageManager::Create();
	SceneManager::Create();
	DebugSystem::Create();
}
/*************************************************
## DeleteSingleton ##
**************************************************/
void Window::DeleteSingleton()
{
	DebugSystem::Delete();
	SceneManager::Delete();
	ImageManager::Delete();
	ImGui::Delete();
	D2DRenderer::Delete();
	D3DRenderer::Delete();
	CameraManager::Delete();
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
	ImGui::Update();
}