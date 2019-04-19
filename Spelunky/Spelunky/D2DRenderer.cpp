#include "stdafx.h"
#include "D2DRenderer.h"

/*****************************************************
## D2DRenderer ##
*******************************************************/
D2DRenderer::D2DRenderer()
	:mD2DFactory(nullptr), mD2DRenderTarget(nullptr), mDWFactory(nullptr)
{
	this->CreateFactory();
	this->CreateRenderTarget();
	this->CreateDefaultBrush();
	this->AddTextFormat(L"맑은고딕");
	this->AddTextFormat(L"Tekton Pro");
}

/*****************************************************
## ~D2DRenderer ##
*******************************************************/
D2DRenderer::~D2DRenderer()
{
	TextIter iter = mFontList.begin();
	for (; iter != mFontList.end(); ++iter)
		SafeRelease(iter->second);

	for (UINT i = 0; i < (UINT)DefaultBrush::End; ++i)
		SafeRelease(mDefaultBrushList[i]);

	SafeRelease(mDWFactory);

	SafeRelease(mD2DSurface);
	SafeRelease(mD2DRenderTarget);
	SafeRelease(mD2DFactory);
}
/*****************************************************
## BeginRender ##
*******************************************************/
void D2DRenderer::BeginRender()
{
	mD2DRenderTarget->BeginDraw();
	mD2DRenderTarget->Clear();
}
/*****************************************************
## EndRender ##
*******************************************************/
void D2DRenderer::EndRender()
{
#ifdef _DEBUG
	mD2DRenderTarget->EndDraw();
#else
	mD2DRenderTarget->EndDraw();
#endif
}

/*****************************************************
## CreateFactory ##
*******************************************************/
void D2DRenderer::CreateFactory()
{
	//D2D 팩토리 생성
	//싱글쓰레드타입으로 
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &mD2DFactory);
#ifdef _DEBUG
	assert(SUCCEEDED(hr));
#endif
	//DWriteFactory 생성
	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(*&mDWFactory),
		(IUnknown**)&mDWFactory
	);
#ifdef _DEBUG
	assert(SUCCEEDED(hr));
#endif
}
/*****************************************************
## CreateRenderTarget ##
*******************************************************/
void D2DRenderer::CreateRenderTarget()
{
	//스왑체인으로부터 백버퍼 정보 받아온다. 
	HRESULT hr = _D3DSwapChain->GetBuffer(0, IID_PPV_ARGS(&mD2DSurface));
	assert(SUCCEEDED(hr));
	FLOAT dpiX;
	FLOAT dpiY;
	//현재 컴퓨터의 dpi정보를 받아온다. 
	mD2DFactory->GetDesktopDpi(&dpiX, &dpiY);
	//위에서 생성한 렌더타겟으로부터 정보를 받아옴
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), dpiX, dpiY);
	//d2dRenderTarget생성 
	hr = mD2DFactory->CreateDxgiSurfaceRenderTarget(mD2DSurface, &props, &mD2DRenderTarget);
	assert(SUCCEEDED(hr));
}
/*****************************************************
## CreateDefaultBrush ##
*******************************************************/
void D2DRenderer::CreateDefaultBrush()
{
	this->mD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black, 1.0f), &mDefaultBrushList[(UINT)DefaultBrush::Black]);
	this->mD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White, 1.0f), &mDefaultBrushList[(UINT)DefaultBrush::White]);
	this->mD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow, 1.0f), &mDefaultBrushList[(UINT)DefaultBrush::Yellow]);
	this->mD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red, 1.0f), &mDefaultBrushList[(UINT)DefaultBrush::Red]);
	this->mD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 1.0f), &mDefaultBrushList[(UINT)DefaultBrush::Blue]);
	this->mD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green, 1.0f), &mDefaultBrushList[(UINT)DefaultBrush::Green]);
	this->mD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray, 1.0f), &mDefaultBrushList[(UINT)DefaultBrush::Gray]);
}
/*****************************************************
## AddTextFormat ##
@@ wstring font : 폰트명
@@ float defaultSize : 폰트 사이즈 = 1.f
*******************************************************/
void D2DRenderer::AddTextFormat(const wstring& font,const float& defaultSize)
{
	HRESULT hr;
	IDWriteTextFormat* format = NULL;

	hr = this->mDWFactory->CreateTextFormat(
		font.c_str(),
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		defaultSize,
		L"ko",
		&format
	);
#ifdef _DEBUG
	assert(SUCCEEDED(hr));
#endif

	this->mFontList.insert(make_pair(font, format));
}


