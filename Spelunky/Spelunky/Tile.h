#pragma once
class Tile
{
	BlockAssign(Tile)
private:
	static float _tileSize;
public:
	static const float& GetTileSize() { return _tileSize; }
public:
	enum class Type : int
	{
		Soil = 0 ,Wood,Rock,Box, Rope,Ladder,Entry,Exit,Empty
	};
private:
	class Image* mImage;
	Figure::FloatRect mRect; 
	Type mType; 
private:
	friend class TileManager;
	friend class MapTool;
	friend class TileMapGenerator;
	Tile() = delete;
	explicit Tile(const float& x, const float& y);
	explicit Tile(const Vector2& pos);
	~Tile();
public:
	void Render();
	void Reset();
public:
	const Figure::FloatRect& GetRect()const { return mRect; }
	const Type& GetType()const { return mType; }
	void SetType(const Type& type) { mType = type; }
	void SetImage(class Image*const pImage);
	void SetImage(const string& key);
	class Image* GetImage()const { return mImage; }
	Vector2 GetPosition() { return mRect.GetCenter(); }
};

