#pragma once


#pragma comment(lib, "dxgi.lib")
#pragma comment(lib,"d3d11.lib")

#ifdef _DEBUG
#pragma comment(lib,"D3DX11.lib")
#else
#pragma comment(lib,"../Libraries/d3dx11.lib")
#endif

#pragma comment(lib,"d3dCompiler.lib")
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")
#pragma comment(lib, "dxguid.lib")
//WIC 라이브러리
#pragma comment(lib, "WindowsCodecs.lib")

#include <dxgi1_2.h>
#include <d3dcommon.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>

#include <directxmath.h>
#include <directxcolors.h>
#include <wincodec.h>

#include <D2D1.h>
#include <d2d1helper.h>
#include <d2d1.h>
#include <dwrite.h>

//lib 링크
#pragma comment (lib, "../Libraries/fmodex_vc.lib")
//fmod.hpp 추가
#include "../Libraries/inc/fmod.hpp"

#pragma comment (lib, "ImGui.lib")
#include "imgui.h"
#include "imguiDx11.h"