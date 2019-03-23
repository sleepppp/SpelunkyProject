#pragma once
#include "SingletonBase.h"

/****************************************************************************
## D2DRenderer ##
@@ Author : 권순우 , Date : 2019.03.20
*****************************************************************************/
class D2DRenderer final : public SingletonBase<D2DRenderer>
{
	BlockAssign(D2DRenderer)
public:
	enum class DefaultBrush : UINT
	{
		White = 0, Black, Yellow, Red, Blue, Green, Gray, End
	};
private:
	typedef unordered_map<wstring, IDWriteTextFormat*>::iterator TextIter;
private:
	ID2D1Factory* mD2DFactory;
	ID2D1RenderTarget*	mD2DRenderTarget ;
	IDXGISurface*		mD2DSurface;

	IDWriteFactory* mDWFactory; 
	
	ID2D1SolidColorBrush* mDefaultBrushList[((UINT)DefaultBrush::End)];
	unordered_map<wstring, IDWriteTextFormat*> mFontList;
private:
	friend class SingletonBase<D2DRenderer>;
	D2DRenderer();
	virtual ~D2DRenderer();
public:
	void BeginRender();
	void EndRender();
private:
	void CreateFactory();
	void CreateRenderTarget();
	void CreateDefaultBrush();
public:
	void AddTextFormat(const wstring& font,const float& defaultSize = 1.f);
	inline ID2D1RenderTarget*const GetRenderTarget()const { return this->mD2DRenderTarget; }
public:
	//텍스트 출력(기본 브러쉬 사용) 
	void RenderText(const int& x,const int& y,const wstring& text,const int& size,
		const DefaultBrush& defaultBrush = DefaultBrush::Black,
		const DWRITE_TEXT_ALIGNMENT& align = DWRITE_TEXT_ALIGNMENT_LEADING,const bool& isRelative = false,
		const wstring& font = L"맑은고딕");
	//텍스트 출력 (원하는 색상과 알파값으로 브러쉬 생성해서 출력)
	void RenderText(const int& x,const int& y,const wstring& text,const COLORREF& color,const float& alpha,const int& size,
		const DWRITE_TEXT_ALIGNMENT& align = DWRITE_TEXT_ALIGNMENT_LEADING,const bool& isRelative = false,
		const wstring& font = L"맑은고딕");
	//텍스트 필드 출력 (기본 브러쉬 사용) 
	void RenderTextField(const int& x,const int& y,const wstring& text,const int& size,const int& width,const int& height,
		const DefaultBrush& defaultBrush = DefaultBrush::Black,
		const DWRITE_TEXT_ALIGNMENT& align = DWRITE_TEXT_ALIGNMENT_LEADING,const bool& isRelative = false,
		const wstring& font = L"맑은고딕");
	//텍스트 필드 출려(원하는 색상과 알파값으로 브러쉬 생성해서 출력)
	void RenderTextField(const int& x, const int& y, const wstring& text, const COLORREF& color,
		const int& size, const int& width, const int& height,
		const float& alpha = 1.0f,
		const DWRITE_TEXT_ALIGNMENT& align = DWRITE_TEXT_ALIGNMENT_LEADING, const bool& isRelative = false,
		const wstring& font = L"맑은고딕");

	void DrawLine(const Vector2& start, const Vector2& end, const D2D1::ColorF::Enum& color, const float& alpha,
		const bool& isRelativePos = false, const float& strokeWidth = 1.f);
	void DrawLine(const Vector2& start, const Vector2& end, const DefaultBrush& defaultBrush = DefaultBrush::Black,
		const bool& isRelativePos = false,const float& strokeWidth = 1.f);
	void DrawRectangle(const Figure::FloatRect& rc, const D2D1::ColorF::Enum& color, const float& alpha,
		const bool& isRelativePos = false, const float& strokeWidth = 1.f);
	void DrawRectangle(const Figure::FloatRect&  rc, const DefaultBrush& defaultBrush = DefaultBrush::Black,
		const bool& isRelativePos = false, const float& strokeWidth = 1.f);
	void DrawEllipse(const Vector2& origin, const float& radius, const DefaultBrush& defaultBrush = DefaultBrush::Black,
		const bool& isARelativePos = false, const float& strokeWidth = 1.f);
	void DrawEllipse(const Vector2& origin, const float& radius, const D2D1::ColorF::Enum& color, const float& alpha,
		const bool& isRelativePos = false, const float& strokeWidth = 1.f);
	void FillRectangle(const Figure::FloatRect&  rc, const  D2D1::ColorF::Enum& color, const float& alpha,
		const bool& isRelativePos = false);
	void FillRectangle(const Figure::FloatRect&  rc, const DefaultBrush& defaultBrush = DefaultBrush::Black,
		const bool& isRelativePos = false);
	void FiilEllipse(const Vector2& origin, const float& radius, const D2D1::ColorF::Enum& color, const float& alpha,
		const bool& isRelative = false);
	void FiilEllipse(const Vector2& origin, const float& radius, const DefaultBrush& brush = DefaultBrush::Black,
		const bool& isRelativePos = false);
};

#define _D2DRenderer D2DRenderer::Get()