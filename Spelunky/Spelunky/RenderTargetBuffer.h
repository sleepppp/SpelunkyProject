#pragma once

class RenderTargetBuffer
{
public:
	RenderTargetBuffer(int width, int height, DXGI_FORMAT format);
	~RenderTargetBuffer();

	ID3D11RenderTargetView* GetRTV() const{ return rtv; }
	ID3D11ShaderResourceView* GetSRV() const{ return srv; }
	ID3D11DepthStencilView* GetDSV() const { return dsv; }
	ID3D11Texture2D* GetTexture2D()const { return this->renderTargetTexture; }

	ID3D11RenderTargetView*const* GetLPRTV() { return &rtv; }
	ID3D11ShaderResourceView*const* GetLPSRV() { return &srv; }
	ID3D11DepthStencilView*const* GetLPDSV() { return &dsv; }
	ID3D11Texture2D*const* GetLPTexture2D() { return &renderTargetTexture; }


	void SetArraySize(int size) { arraySize = size; }
	int GetArraySize() { return arraySize; }
	void SetCubeMap(bool set) { bCubeMap = set; arraySize = 6; }

	void Create();

	void BindRenderTarget();

private:
	void CreateBuffer();
	void CreateArrayBuffer();
	void CreateCubeBuffer();

private:
	int width;
	int height;
	DXGI_FORMAT format;

	int arraySize;
	bool bCubeMap;

	ID3D11Texture2D* renderTargetTexture;
	ID3D11RenderTargetView * rtv;
	ID3D11ShaderResourceView* srv;
	ID3D11DepthStencilView* dsv;
};