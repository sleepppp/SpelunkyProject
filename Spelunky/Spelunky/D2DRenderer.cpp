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
	this->AddTextFormat(L"�������");
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
	//D2D ���丮 ����
	//�̱۾�����Ÿ������ 
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &mD2DFactory);
#ifdef _DEBUG
	assert(SUCCEEDED(hr));
#endif
	//DWriteFactory ����
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
	//����ü�����κ��� ����� ���� �޾ƿ´�. 
	HRESULT hr = _D3DSwapChain->GetBuffer(0, IID_PPV_ARGS(&mD2DSurface));
	assert(SUCCEEDED(hr));
	FLOAT dpiX;
	FLOAT dpiY;
	//���� ��ǻ���� dpi������ �޾ƿ´�. 
	mD2DFactory->GetDesktopDpi(&dpiX, &dpiY);
	//������ ������ ����Ÿ�����κ��� ������ �޾ƿ�
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), dpiX, dpiY);
	//d2dRenderTarget���� 
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
@@ wstring font : ��Ʈ��
@@ float defaultSize : ��Ʈ ������ = 1.f
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


