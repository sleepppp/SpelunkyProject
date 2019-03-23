#pragma once

struct Vector3
{
	union 
	{
		struct
		{
			float x;
			float y; 
			float z;
		};

		float position[3];
	};
	Vector3()
		:x(0), y(0), z(0) {}
	Vector3(float x, float y, float z)
		:x(x), y(y), z(z) {}
};

struct VertexTexture
{
	Vector3 position;
	Vector2	uv;
};

class OrthoWindow 
{
public:
	OrthoWindow(Vector2 pos, Vector2 size);		//UI »ý¼ºÀÚ
	~OrthoWindow();

	void UpdateBuffer();

	void BindBuffer();
	void DrawIndexed();
private:
	Synthesize(Vector2, position,Position)
	Synthesize(Vector2, size,Size)

	Synthesize(ID3D11Buffer*, vertexBuffer, VertexBuffer)
	Synthesize(ID3D11Buffer*, indexBuffer, IndexBuffer)

	vector<VertexTexture> vertexData;
	vector<UINT> indexData;
};
