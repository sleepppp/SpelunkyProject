#include "stdafx.h"
#include "Buffer.h"

void Buffer::CreateVertexBuffer(ID3D11Buffer ** buffer, void* vertex, UINT vertexSize)
{
	D3D11_BUFFER_DESC desc = { 0 };
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = vertexSize;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = vertex;

	HRESULT hr = _D3DDevice->CreateBuffer(&desc, &data, buffer);
	assert(SUCCEEDED(hr));
}

void Buffer::CreateIndexBuffer(ID3D11Buffer ** buffer, UINT* index, UINT indexCount)
{
	D3D11_BUFFER_DESC desc = { 0 };
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(UINT) * indexCount;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = index;

	HRESULT hr = _D3DDevice->CreateBuffer(&desc, &data, buffer);
	assert(SUCCEEDED(hr));
}

void Buffer::CreateDynamicVertexBuffer(ID3D11Buffer ** buffer, void * vertex, UINT vertexSize)
{

	D3D11_BUFFER_DESC desc = { 0 };
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = vertexSize;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = vertex;

	HRESULT hr = _D3DDevice->CreateBuffer(&desc, &data, buffer);
	assert(SUCCEEDED(hr));
}

void Buffer::CreateDynamicIndexBuffer(ID3D11Buffer ** buffer, void * vertex, UINT vertexSize)
{
	D3D11_BUFFER_DESC desc = { 0 };
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = vertexSize;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = vertex;

	HRESULT hr = _D3DDevice->CreateBuffer(&desc, &data, buffer);
	assert(SUCCEEDED(hr));
}

void Buffer::CreateShaderBuffer(ID3D11Buffer ** buffer, void * data, UINT bufferSize)
{
	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = bufferSize;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	HRESULT hr = _D3DDevice->CreateBuffer(&desc, NULL, buffer);
	assert(SUCCEEDED(hr));

}

void Buffer::UpdateBuffer(ID3D11Buffer**buffer, void * data, UINT dataSize)
{
	D3D11_MAPPED_SUBRESOURCE subResource;
	HRESULT hr = _D3DDeviceContext->Map
	(
		*buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
	);

	memcpy(subResource.pData, data, dataSize);
	_D3DDeviceContext->Unmap(*buffer, 0);
}

