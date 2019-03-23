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
	//라이팅 결과 값 저장할 렌더타겟
	renderTargetBuffer = new RenderTargetBuffer(_WinSizeX, _WinSizeY, DXGI_FORMAT_R16G16B16A16_FLOAT);
	renderTargetBuffer->Create();
	//선 렌더링 결과값 카피 할 버퍼
	copyResource = new RenderTargetBuffer(_WinSizeX, _WinSizeY, DXGI_FORMAT_R8G8B8A8_UNORM);
	copyResource->Create();
	//쉐이더 생성
	lightingShader = new Shader(L"Lighting.hlsl");
	deferredShader = new Shader(L"Deferred.hlsl");
	//상수 버퍼 생성
	sunBuffer = new SunLightBuffer;
	pointLightBuffer = new PointLightBuffer;
	//미리 최대 라이팅 수 만큼 벡터 할당
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
	//화면 공간내의 라이트들로 상수버퍼를 갱신
	pointLightBuffer->data.lightSize = this->pointLightList.size();
	for (UINT i = 0; i < pointLightList.size(); ++i)
	{
		//pointLightBuffer->data.lightList[i].lightColor = pointLightList[i]->GetColor();
		//pointLightBuffer->data.lightList[i].origin = _Camera->GetReltativeVector2(pointLightList[i]->GetOrigin());
		//pointLightBuffer->data.lightList[i].originRange = pointLightList[i]->GetOriginRange() * _Camera->GetZoom();
		//pointLightBuffer->data.lightList[i].intensity = pointLightList[i]->GetIntensity();
	}

	//D2D렌더링 및 스왑체인 렌더타겟 바인딩 해제
	//렌더타겟의 자원이 완전히 완성될려면 바인딩을 해제해 주어야 한다. 
	_D2DRenderer->EndRender();
	this->renderTargetBuffer->BindRenderTarget();
	//D2D로 그려낸 장면을 SRV전용 버퍼에 카피(스왑체인에 엮여 있는 자원은 SRV로 사용할 수 없기 때문에 필수 불가결하게 이 방법을 채택)
	_D3DDeviceContext->CopyResource(this->copyResource->GetTexture2D(), _D3DRenderer->GetBackBufferTexture2d());
}
/*********************************************************************************************
## Lighting ##
***********************************************************************************************/
void LightingManager::Lighting()
{
	//화면크기의 장면만을 그려낼 것이기 때문에 정점버퍼 및 인덱스 버퍼는 필요없다. 
	_D3DDeviceContext->IASetInputLayout(nullptr);
	_D3DDeviceContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	_D3DDeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	_D3DDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	{
		//SRV및 상수버퍼,쉐이더 바인딩
		_D3DDeviceContext->PSSetShaderResources(0, 1, copyResource->GetLPSRV());
		sunBuffer->SetPSBuffer(0);
		pointLightBuffer->SetPSBuffer(1);
		lightingShader->Render();
	}
	//렌더링
	_D3DDeviceContext->Draw(4, 0);
}
/*********************************************************************************************
## EndLighting ##
***********************************************************************************************/
void LightingManager::EndLighting()
{
	//스왑체인에 엮여 있는 렌더타겟 다시 바인딩
	_D3DRenderer->BeginRender();
	//화면크기의 장면만을 그려낼 것이기 때문에 정점버퍼 및 인덱스 버퍼는 필요없다. 
	_D3DDeviceContext->IASetInputLayout(nullptr);
	_D3DDeviceContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	_D3DDeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	_D3DDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	{
		//라이팅 결과 버퍼,쉐이더 바인딩
		_D3DDeviceContext->PSSetShaderResources(1, 1, renderTargetBuffer->GetLPSRV());
		deferredShader->Render();
	}
	//렌더링
	_D3DDeviceContext->Draw(4, 0);
	//렌더링이 끝났으므로 라이트 리스트는 해제 
	pointLightList.clear();
	//UI렌더링해야 하므로 D2D렌더링 시작
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
