#pragma once
#include "SingletonBase.h"
/****************************************************************************
## D2DRenderer ##
@@ Author : 권순우 , Date : 2019.03.20
*****************************************************************************/
class D3DRenderer final : public SingletonBase<D3DRenderer>
{
	BlockAssign(D3DRenderer)
private:
	//DirectX11 ----------------------------------------
//그래픽카드 드라이버 타입
	D3D_DRIVER_TYPE mDriverType;
	//direct 지원 레벨 검사해줄 피쳐레벨
	D3D_FEATURE_LEVEL mFeatureLevel;

	ID3D11Device* mD3DDevice;

	ID3D11DeviceContext* mD3DDeviceContext;

	IDXGISwapChain* mD3DSwapChain;

	ID3D11RenderTargetView* mD3DRenderTargetView;
	ID3D11Texture2D* mD3DDepthStencilTexture ;
	ID3D11DepthStencilView* mD3DDepthStencilView;
	ID3D11DepthStencilState* mDepthStencilDisableState;
	ID3D11Texture2D* mD3DBackBufferTexture2D;
private:
	friend class SingletonBase<D3DRenderer>;
	D3DRenderer();
	virtual ~D3DRenderer();
private:
	void CreateDevice();
	void CreateBackBuffer();
	void CreateDepthStencilState();
public:
	void BeginRender();
	void EndRender();
public:
	ID3D11Device*const GetDevice()const { return mD3DDevice; }
	ID3D11DeviceContext*const GetDeviceContext()const { return mD3DDeviceContext; }
	IDXGISwapChain* const GetSwapChain()const { return mD3DSwapChain; }
	ID3D11Texture2D* GetBackBufferTexture2d()const { return this->mD3DBackBufferTexture2D; }
};
#define _D3DRenderer D3DRenderer::Get()
#define _D3DDevice D3DRenderer::Get()->GetDevice()
#define _D3DDeviceContext D3DRenderer::Get()->GetDeviceContext()
#define _D3DSwapChain D3DRenderer::Get()->GetSwapChain()
