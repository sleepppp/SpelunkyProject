#pragma once

class Shader
{
public:
	enum ShaderType : int
	{
		Default = 1, useHS = 2 , useGS = 4 , All = Default | useHS | useGS

	};
public:
	Shader(wstring shaderFile, ShaderType type = ShaderType::Default, string funcName = "");
	~Shader();

	wstring GetFileName() { return shaderFile; }

	void Render();
	void Release();
	void ReleaseShader();
private:
	void CheckShaderError(HRESULT hr, ID3DBlob* error);

	void CreateShaderFromFile(string funcName, ID3DBlob** vsBlob = nullptr);
	void CompileFromFile(string funcName, LPCSTR pProfile, ID3DBlob** ppBlob);
	void CreateInputLayout();
private:
	wstring shaderFile;
	ShaderType useType;

	ID3D11ShaderReflection* reflection;

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11HullShader* hullShader;
	ID3D11DomainShader* domainshader;
	ID3D11GeometryShader* geoShader;

	ID3D11InputLayout* inputLayout;

	ID3D10Blob* vertexBlob;

};

