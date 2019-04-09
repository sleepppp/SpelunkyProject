#include "stdafx.h"
#include "Image.h"

/********************************************************************************
## Image ## 
@@ ID2D1Bitmap* bitmap : ImageManager에서 생성된 비트맵
@@ TagLoadImageInfo loadinfo : 이미지 정보(키값,파일 경로) 

프레임이미지가 아닌 이미지 클래스 생성
*********************************************************************************/
Image::Image(ID2D1Bitmap * const bitmap, const TagLoadedImageInfo & loadinfo)
	:mBitmap(bitmap),mLoadInfo(loadinfo),mScale(1.f),mAlpha(1.f),mAngle(0.f),mMaxFrameX(1),mMaxFrameY(1)
{
	this->mSize.x = (float)this->mBitmap->GetPixelSize().width;
	this->mSize.y = (float)this->mBitmap->GetPixelSize().height;

	FrameRect rc;
	rc.x = 0;
	rc.y = 0;
	rc.width = mSize.x;
	rc.height = mSize.y;
	this->mFrameInfo.push_back(rc);
	this->ResetRenderOption();

}
/********************************************************************************
## Image ##
@@ ID2D1Bitmap* bitmap : ImageManager에서 생성된 비트맵
@@ TagLoadImageInfo loadinfo : 이미지 정보(키값,파일 경로)
@@ int maxFrameX : 가로 프레임 수 
@@ int maxFrameY : 세로 프레임 수 

프레임 이미지 클래스 생성
*********************************************************************************/
Image::Image( ID2D1Bitmap * bitmap, const TagLoadedImageInfo & loadinfo, const int & maxFrameX, const int & maxFrameY)
	:mBitmap(bitmap),mLoadInfo(loadinfo),mMaxFrameX(maxFrameX),mMaxFrameY(maxFrameY), mScale(1.f), mAlpha(1.f), mAngle(0.f)
{
	this->mSize.x = (float)mBitmap->GetPixelSize().width;
	this->mSize.y = (float)mBitmap->GetPixelSize().height;

	float frameX = mSize.x / (float)this->mMaxFrameX;
	float frameY = mSize.y / (float)this->mMaxFrameY;

	FrameRect rc;
	for (int j = 0; j < maxFrameY; ++j)
	{
		for (int i = 0; i < maxFrameX; ++i)
		{
			rc.x = (float)i * (frameX);
			rc.y = (float)j * (frameY);
			rc.width = frameX;
			rc.height = frameY;
			this->mFrameInfo.push_back(rc);
		}
	}

	this->ResetRenderOption();
}
/********************************************************************************
## ~Image ##
비트맵 해제 
*********************************************************************************/
Image::~Image()
{
	SafeRelease(mBitmap);
}
/********************************************************************************
## Render ##
@@ int x : 그릴 좌표 
@@ int y : 그릴 좌표 
@@ Pivot::Enum pivot : 그릴 피봇 
@@ bool isRelative : 카메라 보정 여부 
*********************************************************************************/
void Image::Render(const Vector2& position,const Pivot::Enum& pivot,const bool& isRelativePos)
{
	//그릴 사이즈 = 사이즈 * 스케일
	this->mSize = mSize * mScale;
	//렌더링 좌표
	Vector2 renderPos = GetPivotPosition(position.x, position.y, pivot);

	if (isRelativePos)
	{
		//카메라
		float zoom = _Camera->GetZoom();
		Vector2 zoomScale = Vector2(zoom, zoom);
		mSize.x = mSize.x * zoomScale.x;
		mSize.y = mSize.y * zoomScale.y;

		//카메라 상대좌표
		renderPos = _Camera->GetRelativeVector2(renderPos);
	}
	//렌더링 좌표 기준으로 렉트 생성
	Figure::FloatRect renderRc(renderPos, mSize,Pivot::LeftTop);
	//클리핑영역 
	if (renderRc.left > (float)_WinSizeX || renderRc.right < 0.f ||
		renderRc.top > (float)_WinSizeY || renderRc.bottom < 0.f)
	{
		ResetRenderOption();
		return;
	}
	//크기 정보를 기반으로 scale행렬 구축 
	D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(mScale,mScale);
	Vector2 offset(0, 0);
	//가로로 뒤집을 것인가
	if (mIsReverseAxisX)
	{
		scale = D2D1::Matrix3x2F::Scale(-mScale, mScale);
		offset.x = mSize.x;
	}
	//세로로 뒤집을 것인가
	if (mIsReverseAxisY)
	{
		scale = D2D1::Matrix3x2F::Scale(mScale,-mScale);
		offset.y = mSize.y;
	}
	//angle정보를 기준으로 회전 행렬 구축 
	D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation(mAngle, D2D1::Point2F(mSize.x / 2.f, mSize.y / 2.f));
	//위치 정보와 offset값을 기반으로 이동 행렬 구축
	D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation((renderPos.x + offset.x), (renderPos.y + offset.y));
	//렌더링 영역 생성 
	D2D1_RECT_F dxArea = D2D1::RectF(0.f, 0.f, mSize.x, mSize.y);
	//이미지가 화면에 그려질 최종 행렬은 크기행렬 * 회전행렬 * 이동행렬 
	//행렬은 교환법칙이 성립이 안되므로 반드시 위의 순서를 맞춰서 진행해 주어야 한다. 
	_D2DRenderer->GetRenderTarget()->SetTransform(scale * rotation * translation);
	//렌더링 요청
	_D2DRenderer->GetRenderTarget()->DrawBitmap(mBitmap, dxArea, mAlpha);
	//렌더링이 끝났다면 옵션값 기본으로 세팅
	this->ResetRenderOption();
}
/********************************************************************************
## FrameRender ##
@@ int x : 그릴 좌표 
@@ int y : 그릴 좌표 
@@ int frameX : 가로 프레임 
@@ int frameY : 세로 프레임 
@@ Pivot::Enum pivot : 그릴 피봇 
@@ bool isRalative : 카메라 보정 여부 
*********************************************************************************/
void Image::FrameRender(const Vector2& position,const int& frameX,const int& frameY,const Pivot::Enum& pivot,const bool& isRelativePos)
{
	//프레임이 최대 프레임을 벗어났다면 그리지 않는다. 
	if (frameX >= this->mMaxFrameX || frameY >= this->mMaxFrameY)
	{
		ResetRenderOption();
		return;
	}
	//현재 프레임인덱스 
	int frame = frameY * mMaxFrameX + frameX;
	//사이즈 = 사이즈 * 스케일 
	this->mSize = mSize * mScale;
	//그릴 좌표 
	Vector2 renderPos = GetPivotPosition(position.x, position.y, pivot);

	if (isRelativePos)
	{
		//카메라
		float zoom = _Camera->GetZoom();
		Vector2 zoomScale = Vector2(zoom, zoom);
		mSize.x = mSize.x * zoomScale.x;
		mSize.y = mSize.y * zoomScale.y;

		//카메라 상대좌표
		renderPos = _Camera->GetRelativeVector2(renderPos);
	}
	//카메라 클리핑 
	Figure::FloatRect renderRc(renderPos, mSize, Pivot::LeftTop);
	if (renderRc.left > (float)_WinSizeX || renderRc.right < 0.f ||
		renderRc.top >(float)_WinSizeY || renderRc.bottom < 0.f)
	{
		ResetRenderOption();
		return;
	}
	//크기 행렬 구축 
	D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(mScale, mScale);
	Vector2 offset(0.f, 0.f);
	if (mIsReverseAxisX)
	{
		scale = D2D1::Matrix3x2F::Scale(-mScale, mScale);
		offset.x = mSize.x;
	}
	if (mIsReverseAxisY)
	{
		scale = D2D1::Matrix3x2F::Scale(mScale,-mScale);
		offset.y = mSize.y;
	}
	//회전 행렬 구축 
	D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation(mAngle, D2D1::Point2F(mSize.x / 2.f, mSize.y / 2.f));
	//이동 행렬 구축 
	D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation((renderPos.x + offset.x), (renderPos.y + offset.y));
	//그릴 영역 세팅 
	D2D1_RECT_F dxArea = D2D1::RectF(0.0f, 0.0f, mSize.x, mSize.y);
	D2D1_RECT_F dxSrc = D2D1::RectF((float)mFrameInfo[frame].x, (float)mFrameInfo[frame].y,
		(float)(mFrameInfo[frame].x + mFrameInfo[frame].width),
		(float)(mFrameInfo[frame].y + mFrameInfo[frame].height));
	//최종행렬 세팅
	_D2DRenderer->GetRenderTarget()->SetTransform(scale * rotation * translation);
	//렌더링 요청
	_D2DRenderer->GetRenderTarget()->DrawBitmap(mBitmap, dxArea, mAlpha,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &dxSrc);

	this->ResetRenderOption();
}
/********************************************************************************
## LoopRender ##
@@ Vector2 LeftTop : 시작점
@@ Vector2 RightTop : 끝점 
*********************************************************************************/
void Image::LoopRender(const Vector2 & leftTop, const Vector2 & rightBottom)
{
	Vector2 size = mSize;
	float scale = mScale; 
	Vector2 finalSize = size * scale; 

	for (float y = leftTop.y; y < rightBottom.y; y += finalSize.y)
	{
		for (float x = leftTop.x; x < rightBottom.x; x += finalSize.x)
		{
			this->SetScale(scale);
			this->SetSize(size);
			this->Render(Vector2(x,y), Pivot::LeftTop, true);
		}
	}
}
/********************************************************************************
## ResetRenderOption ##
이미지 클래스 렌더 관련 옵션들 전부 초기화
*********************************************************************************/
void Image::ResetRenderOption()
{
	this->mAlpha = 1.0f;
	this->mScale = 1.0f;
	if (mFrameInfo.size() <= 1)
	{
		this->mSize.x = (float)mBitmap->GetPixelSize().width;
		this->mSize.y = (float)mBitmap->GetPixelSize().height;
	}
	else
	{
		this->mSize.x = mFrameInfo[0].width;
		this->mSize.y = mFrameInfo[0].height;
	}
	this->mIsReverseAxisX = this->mIsReverseAxisY = false;
}
/********************************************************************************
## GetPivotPosition ##
@@ int x : 그릴 좌표 
@@ int y : 그릴 좌표 
@@ Pivot::Enum pivot : 피봇 

피봇과 크기 기반으로 좌표 반환
*********************************************************************************/
const Vector2 Image::GetPivotPosition(const float & x, const float & y, Pivot::Enum pivot)
{
	if (pivot == Pivot::LeftTop)
		return Vector2(x, y);

	Vector2 pos(x, y);

	switch (pivot)
	{
	case Pivot::Center:
		pos.x -= mSize.x * mScale * 0.5f;
		pos.y -= mSize.y * mScale * 0.5f;
		break;

	case Pivot::Bottom:
		pos.x -= mSize.x * mScale * 0.5f;
		pos.y -= mSize.y * mScale;
		break;
	}

	return pos;
}
