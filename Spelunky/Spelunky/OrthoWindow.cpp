#include "stdafx.h"
#include "OrthoWindow.h"

#include "Buffer.h"

OrthoWindow::OrthoWindow(Vector2 pos, Vector2 size)
	:position(pos),size(size)
{	
	UINT vertexCount = 4;
	UINT indexCount = 6;

	vertexData.assign(vertexCount, VertexTexture());
	indexData.assign(indexCount, UINT());

	vertexData[0].position = Vector3(pos.x, pos.y, 0.f);
	vertexData[0].uv = Vector2(0.f, 0.f);

	vertexData[1].position = Vector3(pos.x + size.x, pos.y, 0.f);
	vertexData[1].uv = Vector2(1.f, 0.f);

	vertexData[2].position = Vector3(pos.x, pos.y - size.y, 0.f);
	vertexData[2].uv = Vector2(0.f, 1.f);

	vertexData[3].position = Vector3(pos.x + size.x, pos.y - size.y, 0.f);
	vertexData[3].uv = Vector2(1.f, 1.f);

	indexData[0] = 0;
	indexData[1] = 1;
	indexData[2] = 2;

	indexData[3] = 2;
	indexData[4] = 1;
	indexData[5] = 3;

	Buffer::CreateDynamicVertexBuffer(&vertexBuffer, vertexData.data(), sizeof VertexTexture * vertexCount);
	Buffer::CreateIndexBuffer(&indexBuffer, indexData.data(), indexCount);
}

OrthoWindow::~OrthoWindow()
{
	SafeRelease(vertexBuffer);
	SafeRelease(indexBuffer);

	vertexData.clear();
	indexData.clear();
}

void OrthoWindow::UpdateBuffer()
{
	vertexData[0].position = Vector3(position.x, position.y, 0.f);
	vertexData[1].position = Vector3(position.x + size.x, position.y, 0.f);
	vertexData[2].position = Vector3(position.x, position.y - size.y, 0.f);
	vertexData[3].position = Vector3(position.x + size.x, position.y - size.y, 0.f);

	Buffer::UpdateBuffer(&vertexBuffer, vertexData.data(), sizeof VertexTexture * vertexData.size());
}

void OrthoWindow::BindBuffer()
{
	// ���� ���� ���� �� �������� �����մϴ�.
	unsigned int stride = sizeof(VertexTexture);
	unsigned int offset = 0;

	// ������ �� �� �ֵ��� �Է� ��������� ���� ���۸� Ȱ������ �����մϴ�.
	_D3DDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// ������ �� �� �ֵ��� �Է� ��������� �ε��� ���۸� Ȱ������ �����մϴ�.
	_D3DDeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// �� ������ ���ۿ��� �������Ǿ���ϴ� ������Ƽ�� ������ �����մϴ�.�� ��쿡�� �ﰢ���Դϴ�.
	_D3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void OrthoWindow::DrawIndexed()
{
	_D3DDeviceContext->DrawIndexed(6, 0, 0);
}
