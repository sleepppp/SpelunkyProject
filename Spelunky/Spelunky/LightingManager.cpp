#include "stdafx.h"
#include "LightingManager.h"

#include "RenderTargetBuffer.h"
#include "Shader.h"
#include "OrthoWindow.h"
//#include "PointLight.h"


/*********************************************************************************************
## LightingManager ##
***********************************************************************************************/
LightingManager::LightingManager()
{
	//������ ��� �� ������ ����Ÿ��
	renderTargetBuffer = new RenderTargetBuffer(_WinSizeX, _WinSizeY, DXGI_FORMAT_R16G16B16A16_FLOAT);
	renderTargetBuffer->Create();
	//�� ������ ����� ī�� �� ����
	copyResource = new RenderTargetBuffer(_WinSizeX, _WinSizeY, DXGI_FORMAT_R8G8B8A8_UNORM);
	copyResource->Create();
	//���̴� ����
	lightingShader = new Shader(L"Lighting.hlsl");
	deferredShader = new Shader(L"Deferred.hlsl");
	//��� ���� ����
	sunBuffer = new SunLightBuffer;
	pointLightBuffer = new PointLightBuffer;
	//�̸� �ִ� ������ �� ��ŭ ���� �Ҵ�
	pointLightList.reserve(PointLightBuffer::MaxPointLight);
}

/*********************************************************************************************
## ~LightingManager ##
***********************************************************************************************/
LightingManager::~LightingManager()
{
	pointLightList.clear();

	SafeDelete(pointLightBuffer);
	SafeDelete(sunBuffer);

	SafeDelete(deferredShader);
	SafeDelete(lightingShader);
	
	SafeDelete(copyResource);
	SafeDelete(renderTargetBuffer);
}

/*********************************************************************************************
## BeginLighting ##
***********************************************************************************************/
void LightingManager::BeginLighting()
{
	//ȭ�� �������� ����Ʈ��� ������۸� ����
	pointLightBuffer->data.lightSize = this->pointLightList.size();
	for (UINT i = 0; i < pointLightList.size(); ++i)
	{
		//pointLightBuffer->data.lightList[i].lightColor = pointLightList[i]->GetColor();
		//pointLightBuffer->data.lightList[i].origin = _Camera->GetReltativeVector2(pointLightList[i]->GetOrigin());
		//pointLightBuffer->data.lightList[i].originRange = pointLightList[i]->GetOriginRange() * _Camera->GetZoom();
		//pointLightBuffer->data.lightList[i].intensity = pointLightList[i]->GetIntensity();
	}

	//D2D������ �� ����ü�� ����Ÿ�� ���ε� ����
	//����Ÿ���� �ڿ��� ������ �ϼ��ɷ��� ���ε��� ������ �־�� �Ѵ�. 
	_D2DRenderer->EndRender();
	this->renderTargetBuffer->BindRenderTarget();
	//D2D�� �׷��� ����� SRV���� ���ۿ� ī��(����ü�ο� ���� �ִ� �ڿ��� SRV�� ����� �� ���� ������ �ʼ� �Ұ����ϰ� �� ����� ä��)
	_D3DDeviceContext->CopyResource(this->copyResource->GetTexture2D(), _D3DRenderer->GetBackBufferTexture2d());
}
/*********************************************************************************************
## Lighting ##
***********************************************************************************************/
void LightingManager::Lighting()
{
	//ȭ��ũ���� ��鸸�� �׷��� ���̱� ������ �������� �� �ε��� ���۴� �ʿ����. 
	_D3DDeviceContext->IASetInputLayout(nullptr);
	_D3DDeviceContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	_D3DDeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	_D3DDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	{
		//SRV�� �������,���̴� ���ε�
		_D3DDeviceContext->PSSetShaderResources(0, 1, copyResource->GetLPSRV());
		sunBuffer->SetPSBuffer(0);
		pointLightBuffer->SetPSBuffer(1);
		lightingShader->Render();
	}
	//������
	_D3DDeviceContext->Draw(4, 0);
}
/*********************************************************************************************
## EndLighting ##
***********************************************************************************************/
void LightingManager::EndLighting()
{
	//����ü�ο� ���� �ִ� ����Ÿ�� �ٽ� ���ε�
	_D3DRenderer->BeginRender();
	//ȭ��ũ���� ��鸸�� �׷��� ���̱� ������ �������� �� �ε��� ���۴� �ʿ����. 
	_D3DDeviceContext->IASetInputLayout(nullptr);
	_D3DDeviceContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	_D3DDeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	_D3DDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	{
		//������ ��� ����,���̴� ���ε�
		_D3DDeviceContext->PSSetShaderResources(1, 1, renderTargetBuffer->GetLPSRV());
		deferredShader->Render();
	}
	//������
	_D3DDeviceContext->Draw(4, 0);
	//�������� �������Ƿ� ����Ʈ ����Ʈ�� ���� 
	pointLightList.clear();
	//UI�������ؾ� �ϹǷ� D2D������ ����
	_D2DRenderer->GetRenderTarget()->BeginDraw();
}
/*********************************************************************************************
## OnGui ##
***********************************************************************************************/
void LightingManager::OnGui()
{
	ImGui::Begin("LightSystem");
	{
		ImGui::SliderFloat("WorldAmbient", &sunBuffer->data.worldAmbient, 0.f, 1.f);
		ImGui::SliderFloat("SunIntensity", &sunBuffer->data.sunIntensity, 0.f, 1.f);
		ImGui::ColorEdit4("SunColor", &sunBuffer->data.sunColor.r,
			ImGuiColorEditFlags_Float | ImGuiColorEditFlags_AlphaPreviewHalf);
		ImGui::End();
	}

}
/*********************************************************************************************
## RequestLighting ##
***********************************************************************************************/
void LightingManager::RequestLighting(PointLight *const light)
{
	pointLightList.push_back(light);
}
