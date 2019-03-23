#pragma once
class Buffer
{
public:
	static void CreateVertexBuffer(ID3D11Buffer** buffer, void* vertex, UINT vertexSize);
	static void CreateIndexBuffer(ID3D11Buffer**buffer, UINT* index, UINT indexCount);
	static void CreateDynamicVertexBuffer(ID3D11Buffer** buffer, void* vertex, UINT vertexSize);
	static void CreateDynamicIndexBuffer(ID3D11Buffer** buffer, void* vertex, UINT vertexSize);

	static void CreateShaderBuffer(ID3D11Buffer**buffer, void* data, UINT bufferSize);
	static void UpdateBuffer(ID3D11Buffer**buffer, void* data, UINT dataSize);

};