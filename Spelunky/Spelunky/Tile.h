#pragma once
class Tile
{
	BlockAssign(Tile)
private:
	static float _tileSize;
public:
	static const float& GetTileSize() { return _tileSize; }
private:
	enum class Type : int
	{
		Soil = 0,Wood,Rock,Empty
	};
private:
	class Image* mImage;
	Figure::FloatRect mRect; 
	Type mType; 
private:
	friend class TileManager;
	friend class MapGenerator;
	Tile() = delete;
	explicit Tile(const float& x, const float& y);
	explicit Tile(const Vector2& pos);
	~Tile();
	
	void Render();
	void Reset();
public:
	const Figure::FloatRect& GetRect()const { return mRect; }
	const Type& GetType()const { return mType; }
	void SetType(const Type& type) { mType = type; }
	void SetImage(class Image*const pImage) { mImage = pImage; }
	void SetImage(const string& key);
	Vector2 GetPosition() { return Vector2(mRect.left, mRect.top); }
};

