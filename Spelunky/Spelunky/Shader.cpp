#include "stdafx.h"
#include "Shader.h"

Shader::Shader(wstring shaderFile, ShaderType type , string funcName)
	:shaderFile(shaderFile), useType(type), reflection(nullptr), inputLayout(nullptr),
	vertexShader(nullptr), pixelShader(nullptr),hullShader(nullptr), domainshader(nullptr), geoShader(nullptr) , vertexBlob(nullptr)
{
	CreateShaderFromFile(funcName + "VS", &vertexBlob);
	CreateShaderFromFile(funcName + "PS");
	CreateInputLayout();

	bool b;
	
	if (b = ShaderType::useHS & type)
	{
		CreateShaderFromFile(funcName + "HS");
		CreateShaderFromFile(funcName + "DS");
	}

	if (b = ShaderType::useGS & type)
	{
		CreateShaderFromFile(funcName + "GS");
	}
}

Shader::~Shader()
{
	Release();
}

void Shader::Render()
{
	_D3DDeviceContext->IASetInputLayout(inputLayout);

	_D3DDeviceContext->VSSetShader(vertexShader, NULL, 0);
	_D3DDeviceContext->PSSetShader(pixelShader, NULL, 0);

	bool b;
	if (b = ShaderType::useHS & useType)
	{
		_D3DDeviceContext->HSSetShader(hullShader, NULL, 0);
		_D3DDeviceContext->DSSetShader(domainshader, NULL, 0);
	}

	if (b = ShaderType::useGS & useType)
	{
		_D3DDeviceContext->GSSetShader(geoShader, nullptr, 0);
	}

}

void Shader::Release()
{
	SafeRelease(reflection);
	SafeRelease(inputLayout);

	SafeRelease(vertexShader);
	SafeRelease(pixelShader);
	SafeRelease(vertexBlob);

	bool b;
	if (b = ShaderType::useHS & useType)
	{
		SafeRelease(hullShader);
		SafeRelease(domainshader);
	}

	if (b = ShaderType::useGS & useType)
	{
		SafeRelease(geoShader);
	}
}

void Shader::ReleaseShader()
{
	ID3D11DeviceChild* nullshader = {};

	_D3DDeviceContext->VSSetShader((ID3D11VertexShader*)nullshader, NULL, 0);
	_D3DDeviceContext->PSSetShader((ID3D11PixelShader*)nullshader, NULL, 0);

	bool b;
	if (b = ShaderType::useHS & useType)
	{
		_D3DDeviceContext->HSSetShader((ID3D11HullShader*)nullshader, NULL, 0);
		_D3DDeviceContext->DSSetShader((ID3D11DomainShader*)nullshader, NULL, 0);
	}

	if (b = ShaderType::useGS & useType)
	{
		_D3DDeviceContext->GSSetShader((ID3D11GeometryShader*)nullshader, nullptr, 0);
	}

}

void Shader::CheckShaderError(HRESULT hr, ID3DBlob * error)
{
	if (FAILED(hr))
	{
		if (error != NULL)
		{
			string str = (const char *)error->GetBufferPointer();
			MessageBoxA(NULL, str.c_str(), "Shader Error", MB_OK);
		}
		assert(false);
	}
}

void Shader::CreateShaderFromFile(string funcName, ID3DBlob** vsBlob)
{
	ID3DBlob* blob = nullptr;
	HRESULT hr;

	if (strstr(funcName.c_str(), "VS"))
	{
		CompileFromFile(funcName, "vs_5_0", &blob);

		hr = _D3DDevice->CreateVertexShader
		(
			blob->GetBufferPointer()
			, blob->GetBufferSize()
			, NULL
			, &vertexShader
		);
		assert(SUCCEEDED(hr));

	}
	else if (strstr(funcName.c_str(), "PS"))
	{
		CompileFromFile(funcName, "ps_5_0", &blob);

		hr = _D3DDevice->CreatePixelShader
		(
			blob->GetBufferPointer()
			, blob->GetBufferSize()
			, NULL
			, &pixelShader
		);
		assert(SUCCEEDED(hr));
	}
	else if (strstr(funcName.c_str(), "HS"))
	{
		CompileFromFile(funcName, "hs_5_0", &blob);

		hr = _D3DDevice->CreateHullShader
		(
			blob->GetBufferPointer()
			, blob->GetBufferSize()
			, NULL
			, &hullShader
		);
		assert(SUCCEEDED(hr));
	}
	else if (strstr(funcName.c_str(), "DS"))
	{
		CompileFromFile(funcName, "ds_5_0", &blob);

		hr = _D3DDevice->CreateDomainShader
		(
			blob->GetBufferPointer()
			, blob->GetBufferSize()
			, NULL
			, &domainshader
		);
		assert(SUCCEEDED(hr));
	}
	else if (strstr(funcName.c_str(), "GS"))
	{
		CompileFromFile(funcName, "gs_5_0", &blob);

		hr = _D3DDevice->CreateGeometryShader
		(
			blob->GetBufferPointer()
			, blob->GetBufferSize()
			, NULL
			, &geoShader
		);
		assert(SUCCEEDED(hr));
	}

	if (vsBlob)
		*vsBlob = blob;
	else
		SafeRelease(blob);
}

void Shader::CompileFromFile(string funcName, LPCSTR pProfile, ID3DBlob ** ppBlob)
{
	ID3D10Blob* error;
	HRESULT hr;

	//d3dx11async.h가 d2d와 충돌하므로 사용불가 
	//hr = D3DX11CompileFromFile
	//(
	//	shaderFile.c_str(), NULL, NULL, funcName.c_str(), pProfile
	//	, D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL
	//	, ppBlob, &error, NULL
	//);

	hr = D3DCompileFromFile(shaderFile.c_str(), NULL, NULL, 
		funcName.c_str(), pProfile, D3D10_SHADER_ENABLE_STRICTNESS, 
		0, ppBlob, &error);

	CheckShaderError(hr, error);
	assert(SUCCEEDED(hr));

	SafeRelease(error);

}

void Shader::CreateInputLayout()
{
	//vertexBlob을 이용한 리플렉션 생성 
	HRESULT hr;
	hr = D3DReflect
	(
		vertexBlob->GetBufferPointer()
		, vertexBlob->GetBufferSize()
		, IID_ID3D11ShaderReflection
		, (void**)&reflection
	);
	assert(SUCCEEDED(hr));
	//리플렉션 정보값을 받아온다. 
	D3D11_SHADER_DESC shaderDesc;
	reflection->GetDesc(&shaderDesc);

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	for (UINT i = 0; i< shaderDesc.InputParameters; i++)
	{
		//정점버퍼에 들어갈 파라미터 데이터
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		//인덱스로 받아온다.(정점구조체 파라미터 순서대로 들어있다)
		reflection->GetInputParameterDesc(i, &paramDesc);

		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		string temp = paramDesc.SemanticName;
		if (temp == "POSITION")
			elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;

		inputLayoutDesc.push_back(elementDesc);
	}

	hr = _D3DDevice->CreateInputLayout
	(
		&inputLayoutDesc[0]
		, inputLayoutDesc.size()
		, vertexBlob->GetBufferPointer()
		, vertexBlob->GetBufferSize()
		, &inputLayout
	);
	assert(SUCCEEDED(hr));

}
