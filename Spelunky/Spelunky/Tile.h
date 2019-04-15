#pragma once
#include "GameObject.h"
class Tile : public GameObject
{
	BlockAssign(Tile)
private:
	static float _tileSize;
public:
	 static const float& GetTileSize() { return _tileSize; }
public:
	enum class Type : int
	{
		Empty = 0,
		Decoration ,
		Entry ,
		Soil ,
		Wood ,
		Rock ,
		Gold ,
		Trap ,
		Thorn ,
		Exit 
	};

	enum class TileDirection : int
	{
		Left = 0,Top,Right,Bottom
	};
private:
	struct TagDeco
	{
		Vector2 renderPos;
		ImageInfo imageInfo; 
		TagDeco() {}
	};

private:
	int mIndexX;
	int mIndexY;
	class Image* mImage;
	int mFrameX;
	int mFrameY;
	vector<TagDeco> mDecoList;
	Type mType;
	Figure::FloatRect mRect; 
	class Image* mItemImage; 
	int mItemFrameX;
	int mItemFrameY;
public:
	Tile() = delete;
	explicit Tile(const float& x, const float& y ,const int& indexX,const int& indexY);
	explicit Tile(const Vector2& pos,const int& indexX,const int& indexY);
	~Tile();
public:
	void Init()override;
	void Release()override; 
	void Update()override;
	void Render()override;
	void Reset();
public:
	Figure::FloatRect& GetRect() { return mRect; }
	const Type& GetType()const { return mType; }
	class Image* GetImage()const { return mImage; }
	Vector2 GetPosition() { return mRect.GetCenter(); }
	const int& GetIndexX()const { return mIndexX; }
	const int& GetIndexY()const { return mIndexY; }
	Figure::FloatRect GetCollisionRect()override { return mRect; }

	void SetType(const Type& type);
	void SetImage(class Image*const pImage);
	void SetImage(const string& key);
	void SetFrameX(const int& x) { mFrameX = x; }
	void SetFrameY(const int& y) { mFrameY = y; }
	void SetImageInfo(class Image*const pImage, const int& x, const int& y) { mImage = pImage; mFrameX = x; mFrameY = y; }
	void SetItemInfo(class Image*const pImage, const int& x, const int& y) { mItemImage = pImage; mItemFrameX = x; mItemFrameY = y; }
	void SetDecoInfo(const TileDirection& direction,  class Image* pImage, const int& x, const int& y);

	void SaveData(class BinaryWriter* pWriter);
	void LoadData(class BinaryReader* pReader);
};