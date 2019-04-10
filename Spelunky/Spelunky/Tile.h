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
		Soil = 0 ,Wood,Rock,Gold,Entry,Exit,Trap,Empty
	};
private:
	struct TagDeco
	{
		Vector2 renderPos;
		ImageInfo imageInfo; 
		TagDeco() {}
	};
private:
	class Image* mImage;
	int mFrameX;
	int mFrameY;
	vector<TagDeco> mDecoList;

	Figure::FloatRect mRect; 
	Type mType; 
public:
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
	class Image* GetImage()const { return mImage; }
	Vector2 GetPosition() { return mRect.GetCenter(); }

	void SetType(const Type& type) { mType = type; }
	void SetImage(class Image*const pImage);
	void SetImage(const string& key);
	void SetFrameX(const int& x) { mFrameX = x; }
	void SetFrameY(const int& y) { mFrameY = y; }
	void SetImageInfo(class Image*const pImage, const int& x, const int& y) 
	{ mImage = pImage; mFrameX = x; mFrameY = y; }
	void SetDecoInfo(const Direction::Enum& direction,  class Image* pImage, const int& x, const int& y);
};