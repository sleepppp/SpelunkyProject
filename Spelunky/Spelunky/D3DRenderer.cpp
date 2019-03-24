#include "stdafx.h"
#include "D3DRenderer.h"

/*******************************************************************************************
## D3DRenderer ##
*******************************************************************************************/
D3DRenderer::D3DRenderer()
	:mD3DDevice(nullptr), mD3DDeviceContext(nullptr),mD3DSwapChain(nullptr),
	mD3DRenderTargetView(nullptr),mD3DDepthStencilTexture(nullptr),mD3DDepthStencilView(nullptr),
	mDepthStencilDisableState(nullptr),mD3DBackBufferTexture2D(nullptr)
{
	this->CreateDevice();
	this->CreateBackBuffer();
	this->CreateDepthStencilState();
}
/*******************************************************************************************
## ~D3DRenderer ##
*******************************************************************************************/
D3DRenderer::~D3DRenderer()
{
	SafeRelease(mDepthStencilDisableState);
	SafeRelease(mD3DDepthStencilView);
	SafeRelease(mD3DDepthStencilTexture);
	SafeRelease(mD3DRenderTargetView);
	SafeRelease(mD3DSwapChain);
	SafeRelease(mD3DDeviceContext);
	SafeRelease(mD3DDevice);
}
/*******************************************************************************************
## CreateDevice ##
*******************************************************************************************/
void D3DRenderer::CreateDevice()
{
	HRESULT hr = S_OK;

	Window::Desc windowDesc;
	Window::GetDesc(&windowDesc);
	//클라이언트 영역 받아온다. 
	HWND handle = windowDesc.handle;
	RECT rc;
	GetClientRect(handle, &rc);

	UINT width = (UINT)(rc.right - rc.left);
	UINT height = (UINT)(rc.bottom - rc.top);
	//디바이스 플래그 세팅
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	//드라이버 타입 고사양부터 저사양으로 순차적 접근
	D3D_DRIVER_TYPE mDriverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT nummDriverTypes = ARRAYSIZE(mDriverTypes);
	//해당 컴이 지원하는 direct레벨 체크 
	D3D_FEATURE_LEVEL mFeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT nummFeatureLevels = ARRAYSIZE(mFeatureLevels);
	//해당 그래픽 드라이버 지원 레벨 체크 
	for (UINT mDriverTypeIndex = 0; mDriverTypeIndex < nummDriverTypes; mDriverTypeIndex++)
	{
		//해당 드라이버 타입으로 디바이스 생성 시도 
		this->mDriverType = mDriverTypes[mDriverTypeIndex];
		hr = D3D11CreateDevice(nullptr, mDriverType, nullptr, createDeviceFlags, mFeatureLevels, nummFeatureLevels,
			D3D11_SDK_VERSION, &mD3DDevice, &this->mFeatureLevel, &mD3DDeviceContext);

		if (hr == E_INVALIDARG)
		{
			hr = D3D11CreateDevice(nullptr, mDriverType, nullptr, createDeviceFlags, &mFeatureLevels[1], nummFeatureLevels - 1,
				D3D11_SDK_VERSION, &mD3DDevice, &mFeatureLevel, &mD3DDeviceContext);
		}
		//생성에 성공 했다면 해당 버젼으로 생성한 디바이스를 사용할 것이므로 빠져나가자 
		if (SUCCEEDED(hr))
			break;
	}
	//실패했으면 에러 메세지 
	assert(SUCCEEDED(hr));
	//팩토리 및 어뎁터 생성 부분 
	IDXGIFactory1* dxgiFactory = nullptr;
	{
		IDXGIDevice* dxgiDevice = nullptr;
		//생성한 디바이스로 어뎁터 생성가능 한지 체킹 
		hr = mD3DDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		if (SUCCEEDED(hr))
		{
			//디바이스 내의 어뎁터 가져온다. 
			IDXGIAdapter* adapter = nullptr;
			hr = dxgiDevice->GetAdapter(&adapter);
			if (SUCCEEDED(hr))
			{
				//어뎁터에서 팩토리를 받아온다. 
				hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
				adapter->Release();
			}
			dxgiDevice->Release();
		}
	}
	//실패했으면 에러 메세지 
	assert(SUCCEEDED(hr));
	
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = windowDesc.handle;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	hr = dxgiFactory->CreateSwapChain(mD3DDevice, &sd, &mD3DSwapChain);
	assert(SUCCEEDED(hr));

	//-----------------------------------------------------------------------------------------------------------------------
	dxgiFactory->MakeWindowAssociation(windowDesc.handle, DXGI_MWA_NO_ALT_ENTER);
	dxgiFactory->Release();
}
/*******************************************************************************************
## CreateBackBuffer ##
*******************************************************************************************/
void D3DRenderer::CreateBackBuffer()
{
	//클라이언트 영역 받아옴
	HWND handle = Window::GetHandle();
	RECT rc;
	GetClientRect(handle, &rc);

	UINT width = (UINT)(rc.right - rc.left);
	UINT height = (UINT)(rc.bottom - rc.top);

	HRESULT hr;
	//생성한 스왑체인으로 부터 백버퍼 정보를 받아온다, 
	hr = mD3DSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&mD3DBackBufferTexture2D));
	assert(SUCCEEDED(hr));
	//렌더타겟뷰생성 
	hr = mD3DDevice->CreateRenderTargetView(mD3DBackBufferTexture2D, nullptr,
		&mD3DRenderTargetView);

	//pBackBuffer->Release();
	assert(SUCCEEDED(hr));
	//렌더타겟용 텍스처 2d생성
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = mD3DDevice->CreateTexture2D(&descDepth, nullptr, &mD3DDepthStencilTexture);
	assert(SUCCEEDED(hr));

	//깊이 스텐실 뷰 생성
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = mD3DDevice->CreateDepthStencilView(mD3DDepthStencilTexture, &descDSV, &mD3DDepthStencilView);
	assert(SUCCEEDED(hr));

	mD3DDeviceContext->OMSetRenderTargets(1, &mD3DRenderTargetView, mD3DDepthStencilView);
	//뷰포트 설정
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	mD3DDeviceContext->RSSetViewports(1, &vp);
}

/*******************************************************************************************
## CreateDepthStencilState ##
*******************************************************************************************/
void D3DRenderer::CreateDepthStencilState()
{
	// 스텐실 사용 안함
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc = { 0 };
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	HRESULT hr = mD3DDevice->CreateDepthStencilState(&depthDisabledStencilDesc, &mDepthStencilDisableState);
	assert(SUCCEEDED(hr));

	mD3DDeviceContext->OMSetDepthStencilState(mDepthStencilDisableState, 1);
}
/*******************************************************************************************
## BeginRender ##
*******************************************************************************************/
void D3DRenderer::BeginRender()
{
	//RenderTargetView Clear해준다.
	float color[4] = { 0.3f,0.3f,0.3f,1.0f };
	mD3DDeviceContext->ClearRenderTargetView(mD3DRenderTargetView, color);
	//깊이 버퍼와 렌더타겟 뷰 바인딩
	mD3DDeviceContext->OMSetRenderTargets(1, &mD3DRenderTargetView, nullptr);
}
/*******************************************************************************************
## EndRender ##
*******************************************************************************************/
void D3DRenderer::EndRender()
{
	mD3DSwapChain->Present(0, 0);
}


