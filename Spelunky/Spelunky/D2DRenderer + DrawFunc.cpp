#include "stdafx.h"
#include "D2DRenderer.h"

/**********************************************************************************************
## RenderText ##
@@ int x : x ��ǥ
@@ int y : y ��ǥ 
@@ wstring text : �׸� �ؽ�Ʈ 
@@ int size : ��Ʈ ������ 
@@ DefaultBrush brush : �׸� �귯�� = DefaultBrush::Black
@@ DWRITE_TEXT_ALIGNMENT align : ���� �ɼ� = DWRITE_TEXT_ALIGNMENT_LEADING
@@ bool isRelative : ī�޶� ���� ���� = false
@@ wstring font : ��Ʈ = �������

�⺻ �����ص� �귯���� �ؽ�Ʈ ������ 
************************************************************************************************/
void D2DRenderer::RenderText(const int & x, const int & y, const wstring & text, const int & size, 
	const DefaultBrush & defaultBrush, const DWRITE_TEXT_ALIGNMENT & align,
	const bool & isRelative, const wstring & font)
{
	Vector2 pos(x, y);
	if (isRelative)
	{
		pos = _Camera->GetRelativeVector2(pos);
	}
	
	IDWriteTextLayout* layout = nullptr;
	HRESULT hr = this->mDWFactory->CreateTextLayout
	(
		text.c_str(),
		text.length(),
		this->mFontList[font],
		(float)text.length() * size,
		(float)size,
		&layout
	);
	assert(SUCCEEDED(hr));

	DWRITE_TEXT_RANGE range;
	range.startPosition = 0;
	range.length = text.length();

	layout->SetFontSize((float)size, range);
	layout->SetTextAlignment(align);

	mD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	mD2DRenderTarget->DrawTextLayout(D2D1::Point2F(pos.x, pos.y), layout,
		mDefaultBrushList[(UINT)defaultBrush]);

	SafeRelease(layout);
}
/**********************************************************************************************
## RenderText ##
@@ int x : �׸���ǥ
@@ int y : �׸���ǥ 
@@ wstring text : �׸� �ؽ�Ʈ 
@@ COLORREF color : RGB�÷� 
@@ float alpha : ���� �� 
@@ int size : ��Ʈ ������
@@ DWRITE_TEXT_ALIGNMENT align : ���Ŀɼ� 
@@ bool isRelative : ī�޶� ���� ���� 
@@ wstring font : ��Ʈ 

���ο� �귯�� ������ ������
************************************************************************************************/
void D2DRenderer::RenderText(const int & x, const int & y, const wstring & text, const COLORREF & color, 
	const float & alpha, const int & size, const DWRITE_TEXT_ALIGNMENT & align,
	const bool & isRelative, const wstring & font)
{
	Vector2 pos(x, y);
	if (isRelative)
	{
		pos = _Camera->GetRelativeVector2(pos);
	}
	IDWriteTextLayout* layout = nullptr;
	//�ؽ�Ʈ ���̾ƿ� ����
	mDWFactory->CreateTextLayout(
		text.c_str(),
		text.length(),
		mFontList[font],
		(float)text.length() * size,
		(float)size,
		&layout
	);

	//���̾ƿ� �¾�
	DWRITE_TEXT_RANGE range;
	range.startPosition = 0;
	range.length = text.length();
	layout->SetFontSize((float)size, range);
	layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	layout->SetTextAlignment(align);

	//�귯�� ����
	ID2D1SolidColorBrush* brush;
	mD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);

	mD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	mD2DRenderTarget->DrawTextLayout(D2D1::Point2F(pos.x, pos.y), layout, brush);

	SafeRelease(brush);
	SafeRelease(layout);
}
/**********************************************************************************************
## RenderTextField ##
@@ int x : �׸� ��ǥ 
@@ int y : �׸� ��ǥ 
@@ wstring text : �׸� �ؽ�Ʈ 
@@ int size : ��Ʈ ������
@@ int width : ���� ���α���
@@ int heifht : ���� ���α��� 
@@ DefaultBrush brush : �⺻ ���� �귯�� 
@@ DWRITE_TEXT_ALIGNMENT align : ���� �ɼ� 
@@ bool isRalative : ī�޶� ���� ���� 
@@ wstring font : ��Ʈ 

���������� �⺻ �귯���� ���� ��� 
************************************************************************************************/
void D2DRenderer::RenderTextField(const int & x, const int & y, const wstring & text, const int & size, 
	const int & width, const int & height, const DefaultBrush & defaultBrush, const DWRITE_TEXT_ALIGNMENT & align, 
	const bool & isRelative, const wstring & font)
{
	Vector2 pos(x, y);
	if (isRelative)
	{
		pos = _Camera->GetRelativeVector2(pos);
	}
	IDWriteTextLayout* layout = nullptr;
	mDWFactory->CreateTextLayout(
		text.c_str(),
		text.length(),
		mFontList[font],
		(float)width,
		(float)height,
		&layout
	);

	//���̾ƿ� �¾�
	DWRITE_TEXT_RANGE range;
	range.startPosition = 0;
	range.length = text.length();
	layout->SetFontSize((float)size, range);

	layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	layout->SetTextAlignment(align);

	mD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	mD2DRenderTarget->DrawTextLayout(D2D1::Point2F(pos.x, pos.y), layout, mDefaultBrushList[(UINT)defaultBrush]);

	SafeRelease(layout);
}
/**********************************************************************************************
## RenderText ##
@@ int x : �׸� ��ǥ 
@@ int y : �׸� ��ǥ 
@@ wstring text : �׸� �ؽ�Ʈ 
@@ COLORREF color : �׸� �÷� 
@@ int size : ��Ʈ ������ 
@@ int width : ���� ���� ���� 
@@ int height : ���� ���� ���� 
@@ float alpha : ���İ� 
@@ DWRITE_TEXT_ALIGNMENT align : ���� �ɼ� 
@@ bool isRelative : ī�޶� �������� 
@@ wstring font : ��Ʈ 

�����ȿ��� ���ο� �귯���� �����ؼ� ��Ʈ��� 
************************************************************************************************/
void D2DRenderer::RenderTextField(const int & x, const int & y, const wstring & text, const COLORREF & color,
	const int & size, const int & width, const int & height, const float & alpha, const DWRITE_TEXT_ALIGNMENT & align, 
	const bool & isRelative, const wstring & font)
{
	Vector2 pos(x, y);
	if (isRelative)
	{
		pos = _Camera->GetRelativeVector2(pos);
	}
	IDWriteTextLayout* layout = nullptr;
	mDWFactory->CreateTextLayout(
		text.c_str(),
		text.length(),
		mFontList[font],
		(float)width,
		(float)height,
		&layout
	);

	//���̾ƿ� �¾�
	DWRITE_TEXT_RANGE range;
	range.startPosition = 0;
	range.length = text.length();
	layout->SetFontSize((float)size, range);

	layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	layout->SetTextAlignment(align);

	//�귯�� ����
	ID2D1SolidColorBrush* brush;
	mD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);

	mD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	mD2DRenderTarget->DrawTextLayout(D2D1::Point2F((float)x, (float)y), layout, brush);

	SafeRelease(layout);
	SafeRelease(brush);
}
/**********************************************************************************************
## DrawLine ##
@@ Vector2 start : ���� �� 
@@ Vector2 end : �� �� 
@@ D2D1::ColorF::Enum color : ���� 
@@ float alpha : ���� ��
@@ bool isRelative : ī�޶� ���� ���� 
@@ float strokeWidth : �� ���� 
************************************************************************************************/
void D2DRenderer::DrawLine(const Vector2 & start, const Vector2 & end, const D2D1::ColorF::Enum & color, 
	const float & alpha, const bool & isRelativePos, const float & strokeWidth)
{
	ID2D1SolidColorBrush* brush;
	mD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);

	mD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	Vector2 startPos = start;
	Vector2 endPos = end;
	if (isRelativePos)
	{
		startPos = _Camera->GetRelativeVector2(start);
		endPos = _Camera->GetRelativeVector2(end);
	}

	mD2DRenderTarget->DrawLine(D2D1::Point2F(startPos.x, startPos.y), D2D1::Point2F(endPos.x, endPos.y), brush, strokeWidth);
	SafeRelease(brush);
}
/**********************************************************************************************
## RenderText ##
@@ Vector2 start : ���� �� 
@@ Vector2 end : �� �� 
@@ DefaultBrush brush : �⺻ ���� �귯�� 
@@ bool isRelative : ī�޶� ���� ���� 
@@ float stroke : �� ���� 
***********************************************************************************************/
void D2DRenderer::DrawLine(const Vector2 & start, const Vector2 & end, const DefaultBrush & defaultBrush, 
	const bool & isRelativePos, const float & strokeWidth)
{
	mD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	Vector2 startPos = start;
	Vector2 endPos = end;
	if (isRelativePos)
	{
		startPos = _Camera->GetRelativeVector2(start);
		endPos = _Camera->GetRelativeVector2(end);
	}

	mD2DRenderTarget->DrawLine(D2D1::Point2F(startPos.x, startPos.y),
		D2D1::Point2F(endPos.x, endPos.y), mDefaultBrushList[(UINT)defaultBrush], strokeWidth);

}
/**********************************************************************************************
## DrawRectangle ##
@@ Figure::FloatRect rc : ��Ʈ 
@@ D2D1::ColorF::Enum color : �÷� 
@@ float alpha : ���� �� 
@@ bool isRelative : ī�޶� �������� 
@@ float strokeWidth : �� ���� 
************************************************************************************************/
void D2DRenderer::DrawRectangle(const Figure::FloatRect & rc, const D2D1::ColorF::Enum & color, 
	const float & alpha, const bool & isRelativePos, const float & strokeWidth)
{
	Figure::FloatRect rect = rc;
	if (isRelativePos)
	{
		rect = _Camera->GetRelativeRect(rect);
	}
	//ī�޶� ������ ����x
	if (rect.left > (float)_WinSizeX || rect.right < 0.f ||
		rect.top  >(float)_WinSizeY || rect.bottom < 0.f)
	{
		return;
	}

	ID2D1SolidColorBrush* brush;
	mD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);

	mD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	mD2DRenderTarget->DrawRectangle(D2D1::RectF((float)rect.left, (float)rect.top, (float)rect.right, (float)rect.bottom),
		brush, strokeWidth);

	SafeRelease(brush);
}
/**********************************************************************************************
## DrawRectangle ##
@@ Figure::FloatRect rc : �׸� ��Ʈ 
@@ DefaultBrush brush : �׸� �귯�� 
@@ bool isRelative : ī�޶� ���� ���� 
@@ float stroke : �� ����
************************************************************************************************/
void D2DRenderer::DrawRectangle(const Figure::FloatRect & rc, const DefaultBrush & defaultBrush, 
	const bool & isRelativePos, const float & strokeWidth)
{
	Figure::FloatRect rect = rc;
	if (isRelativePos)
	{
		rect = _Camera->GetRelativeRect(rect);
	}
	//ī�޶� ������ ����x
	if (rect.left > (float)_WinSizeX || rect.right < 0.f ||
		rect.top  >(float)_WinSizeY || rect.bottom < 0.f)
	{
		return;
	}

	mD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	mD2DRenderTarget->DrawRectangle(D2D1::RectF((float)rect.left, (float)rect.top, (float)rect.right, (float)rect.bottom),
		mDefaultBrushList[(UINT)defaultBrush], strokeWidth);

}
/**********************************************************************************************
## DrawEllipse ##
@@ Vector2 origin : ����
@@ flaot radius : ������ 
@@ DefaultBrush brush : �귯�� 
@@ bool isRelative : ī�޶� �������� 
@@ float stroke : �� ���� 
************************************************************************************************/
void D2DRenderer::DrawEllipse(const Vector2 & origin, const float & radius, const DefaultBrush & defaultBrush,
	const bool & isARelativePos, const float & strokeWidth)
{
	Figure::FloatRect rc = Figure::RectMakePivot(origin, Vector2(radius,radius),Pivot::Center);
	Vector2 pos = origin;
	if (isARelativePos)
	{
		rc = _Camera->GetRelativeRect(rc);
		pos = _Camera->GetRelativeVector2(pos);
	}
	//ī�޶� ������ ����x
	if (rc.left > (float)_WinSizeX || rc.right < 0.f ||
		rc.top  >(float)_WinSizeY || rc.bottom < 0.f)
	{
		return;
	}

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = pos.x;
	ellipse.point.y = pos.y;
	ellipse.radiusX = radius;
	ellipse.radiusY = radius;

	mD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	mD2DRenderTarget->DrawEllipse(&ellipse, mDefaultBrushList[(UINT)defaultBrush], strokeWidth);
}
/**********************************************************************************************
## DrawEllipse ##
@@ Vector2 origin : ���� 
@@ flaot radius : ������
@@ D2D1::ColorF::Enum color : D2D�÷� �� 
@@ float alpha : ���� �� 
@@ bool isRelative : ī�޶� ���� ���� 
@@ float stroke : �� ���� 
************************************************************************************************/
void D2DRenderer::DrawEllipse(const Vector2 & origin, const float & radius, const D2D1::ColorF::Enum & color,
	const float & alpha, const bool & isRelativePos, const float & strokeWidth)
{
	Figure::FloatRect rc = Figure::RectMakePivot(origin, Vector2(radius, radius), Pivot::Center);
	Vector2 pos = origin;
	if (isRelativePos)
	{
		rc = _Camera->GetRelativeRect(rc);
		pos = _Camera->GetRelativeVector2(pos);
	}
	//ī�޶� ������ ����x
	if (rc.left > (float)_WinSizeX || rc.right < 0.f ||
		rc.top  >(float)_WinSizeY || rc.bottom < 0.f)
	{
		return;
	}

	ID2D1SolidColorBrush* brush(nullptr);
	mD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = pos.x;
	ellipse.point.y = pos.y;
	ellipse.radiusX = radius;
	ellipse.radiusY = radius;

	mD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	mD2DRenderTarget->DrawEllipse(&ellipse, brush, strokeWidth);

	SafeRelease(brush);
}
/**********************************************************************************************
## FillRectagle ##
@@ Figure::FloatRect rc : �׸� ��Ʈ 
@@ D2D1::ColorF::Enum color : D2D�÷� 
@@ float alpha  : ���� �� 
@@ bool isRelative : ī�޶� ���� ����
************************************************************************************************/
void D2DRenderer::FillRectangle(const Figure::FloatRect & rc, const D2D1::ColorF::Enum & color, const float & alpha,
	const bool & isRelativePos)
{
	Figure::FloatRect rect = rc;
	if (isRelativePos)
	{
		rect = _Camera->GetRelativeRect(rect);
	}
	if (rect.left > (float)_WinSizeX || rect.right < 0.f ||
		rect.top  >(float) _WinSizeY || rect.bottom < 0.f)
	{
		return;
	}

	ID2D1SolidColorBrush* brush;
	mD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);
	mD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	mD2DRenderTarget->FillRectangle(D2D1::RectF((float)rect.left, (float)rect.top, (float)rect.right, (float)rect.bottom), brush);

	SafeRelease(brush);
}
/**********************************************************************************************
## FillRectangle  ##
@@ Figure::FloatRec rc : ũ�� ��Ʈ 
@@ DefaultBrush brush : �귯�� 
@@ bool isRelative : ī�޶� ��������
************************************************************************************************/
void D2DRenderer::FillRectangle(const Figure::FloatRect & rc, const DefaultBrush & defaultBrush, 
	const bool & isRelativePos)
{
	Figure::FloatRect rect = rc;
	if (isRelativePos)
	{
		rect = _Camera->GetRelativeRect(rect);
	}
	if (rect.left > (float)_WinSizeX || rect.right < 0.f ||
		rect.top  >(float) _WinSizeY || rect.bottom < 0.f)
	{
		return;
	}

	mD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	mD2DRenderTarget->FillRectangle(D2D1::RectF((float)rect.left, (float)rect.top, (float)rect.right, (float)rect.bottom),
		mDefaultBrushList[(UINT)defaultBrush]);
}
/**********************************************************************************************
## FillEllipse ##
@@ Vector2 origin : ���� ��ǥ 
@@ flaot radius : ������ 
@@ D2D1::ColorF::Enum color : D2D�÷� 
@@ float alpha : ���� �� 
@@ bool isRelative : ī�޶� ���� ���� 
************************************************************************************************/
void D2DRenderer::FiilEllipse(const Vector2 & origin, const float & radius, const D2D1::ColorF::Enum & color,
	const float & alpha, const bool & isRelative)
{
	Figure::FloatRect rc = Figure::RectMakePivot(origin, Vector2(radius, radius), Pivot::Center);
	Vector2 pos = origin;
	if (isRelative)
	{
		rc = _Camera->GetRelativeRect(rc);
		pos = _Camera->GetRelativeVector2(pos);
	}
	//ī�޶� ������ ����x
	if (rc.left > (float)_WinSizeX || rc.right < 0.f ||
		rc.top  >(float)_WinSizeY || rc.bottom < 0.f)
	{
		return;
	}
	ID2D1SolidColorBrush* brush(nullptr);
	mD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = pos.x;
	ellipse.point.y = pos.y;
	ellipse.radiusX = radius;
	ellipse.radiusY = radius;

	mD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	mD2DRenderTarget->FillEllipse(&ellipse, brush);
	SafeRelease(brush);
}
/**********************************************************************************************
## FillEllipse ##
@@ Vector2 origin : ���� ��ǥ
@@ float radius : ������ 
@@ DefaultBrush brush : �귯�� 
@@ bool isRlative : ī�޶� ���� ����
************************************************************************************************/
void D2DRenderer::FiilEllipse(const Vector2 & origin, const float & radius, const DefaultBrush & brush, 
	const bool & isRelativePos)
{
	Figure::FloatRect rc = Figure::RectMakePivot(origin, Vector2(radius, radius), Pivot::Center);
	Vector2 pos = origin;
	if (isRelativePos)
	{
		rc = _Camera->GetRelativeRect(rc);
		pos = _Camera->GetRelativeVector2(pos);
	}
	//ī�޶� ������ ����x
	if (rc.left > (float)_WinSizeX || rc.right < 0.f ||
		rc.top  >(float)_WinSizeY || rc.bottom < 0.f)
	{
		return;
	}
	D2D1_ELLIPSE ellipse;
	ellipse.point.x = pos.x;
	ellipse.point.y = pos.y;
	ellipse.radiusX = radius;
	ellipse.radiusY = radius;

	mD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	mD2DRenderTarget->FillEllipse(&ellipse, mDefaultBrushList[(UINT)brush]);
}
