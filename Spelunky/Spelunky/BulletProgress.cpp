#include "stdafx.h"
#include "BulletProgress.h"

#include "Transform.h"
BulletProgress::BulletProgress(const wstring& text,const Vector2 & pos, const Vector2 & size)
	:UIModule(pos, size), mText(text), mTotalCount(20),mCurrentCount(10)
{
	mBackgroundImage = _ImageManager->FindImage("UIBaseGray");
	mBulletImage = _ImageManager->FindImage("SelectBulletUI");

	for (int i = 0; i < mTotalCount; ++i)
	{
		Figure::FloatRect rc = mTransform->GetRect();
		mBullets[i].Update(Vector2(rc.left + 20 + 15 + 28 * i, rc.GetCenter().y),
			Vector2(25, 40), Pivot::Center);
	}
}

BulletProgress::~BulletProgress()
{
}

void BulletProgress::Update()
{
	if (_Input->GetKey(VK_LBUTTON))
	{
		Vector2 mouse = _Input->GetMousePosition();
		for (int i = 0; i < mTotalCount; ++i)
		{
			if (Figure::Vector2InRect(&mBullets[i], &mouse))
			{
				if (mCurrentCount != i)
				{
					mCurrentCount = Math::Clamp(i,1,mTotalCount - 1);
					_SoundManager->Play("ButtonOnMouse");
					if (mFunc)
					{
						float ratio = (float)mCurrentCount / (float)mTotalCount;
						mFunc(ratio);
					}
				}
				break;
			}
		}
	}
}

void BulletProgress::Render()
{
	Figure::FloatRect rc = mTransform->GetRect();
	_D2DRenderer->RenderTextField(CastingInt(rc.left), CastingInt(rc.top) - 50, mText, 30, CastingInt(rc.GetWidth()), CastingInt(rc.GetHeight()),
		D2DRenderer::DefaultBrush::Gray, DWRITE_TEXT_ALIGNMENT_CENTER, false, L"Tekton Pro");

	mBackgroundImage->SetSize(mTransform->GetSize());
	mBackgroundImage->Render(mTransform->GetWorldPosition(), Pivot::Center, false);

	for (int i = 0; i < mTotalCount; ++i)
	{
		int frameX = 0;
		if (i >= mCurrentCount)
			frameX = 1;

		mBulletImage->SetSize(mBullets[i].GetSize());
		mBulletImage->FrameRender(mBullets[i].GetCenter(),frameX,0, Pivot::Center, false);

		if (_isDebug)
			_D2DRenderer->DrawRectangle(mBullets[i], D2DRenderer::DefaultBrush::Red, false, 2.f);
	}
}

void BulletProgress::SetRatio(const float & ratio)
{
	mCurrentCount = CastingInt(ratio * (float)mTotalCount);
}
