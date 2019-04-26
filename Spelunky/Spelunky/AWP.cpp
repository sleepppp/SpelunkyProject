#include "stdafx.h"
#include "AWP.h"
#include "Transform.h"
#include "Unit.h"
#include "TileManager.h"
#include "Monster.h"

#include "Image.h"
#include "Unit.h"
#include "FrameEffecter.h"
#include "PointLight.h"

const float AWP::_maxRange = 1000.f;

const float AWP::_startGunFireRange = 300.f;
const float AWP::_endGunFireRange = 30.f;
const float AWP::_startGunFireIntensity = 5.0f;
const float AWP::_endGunFireIntensity = 0.1f;

AWP::AWP(const Vector2& worldPos,const bool& installation)
	:Item(worldPos, installation)
{
	mImageInfo.image = _ImageManager->FindImage("AWP");
}


AWP::~AWP()
{
}

void AWP::Init()
{
	Item::Init();
	mTileManager = (TileManager*)_World->GetObjectPool()->FindObject("TileManager");

	mEffecter = reinterpret_cast<FrameEffecter*>(_World->GetObjectPool()->FindObject("FrameEffecter"));

	mPointLight = new PointLight(mTransform->GetWorldPosition());
	mPointLight->SetActive(false);
	mPointLight->Init();
	mPointLight->SetColor(GameColor(1.f, 0.94f, 0.7f, 1.f));
}

void AWP::Update()
{
	Item::Update();
	if (mIsFire)
	{
		mCurrentDelay += _TimeManager->DeltaTime();
		if (mCurrentDelay >= 1.f)
		{
			mPointLight->SetActive(false);
			mIsFire = false;
		}
		else
		{
			float normalizeTime = mCurrentDelay / 1.f;

			float currentRange = _startGunFireRange - (_startGunFireRange - _endGunFireRange) * normalizeTime;
			float currentIntensity = _startGunFireIntensity - (_startGunFireIntensity - _endGunFireIntensity) * normalizeTime;
			mPointLight->SetRange(currentRange);
			mPointLight->SetIntensity(currentIntensity);
		}
	}
	if (mUnit)
	{
		float tileSize = Tile::GetTileSize();
		float probingRange = tileSize * 0.5f;
		Vector2 direction = mUnit->GetAimDirection();
		Vector2 origin = mTransform->GetCenterPos() + direction * (mImageInfo.image->GetFrameSize().x * 0.1f);
		Vector2 tempPos = origin;
		while (true)
		{
			tempPos = tempPos + direction * probingRange;
			int indexX = CastingInt(tempPos.x / tileSize);
			int indexY = CastingInt(tempPos.y / tileSize);
			if (Tile* tile = mTileManager->GetTile(indexX, indexY))
			{
				Tile::Type type = tile->GetType();
				if (type != Tile::Type::Empty &&
					type != Tile::Type::Thorn &&
					type != Tile::Type::Decoration)
				{
					Figure::Line line(origin, tempPos);
					if (Figure::IntersectLineToRect(&endPos, line, tile->GetRect()))
					{
						break;
					}
				}
			}

			if (Vector2::Length(&(tempPos - origin)) > _maxRange)
			{
				endPos = origin + direction * _maxRange;
				break;
			}
		}
	}
}

void AWP::Render()
{
	Item::Render();
	if (mUnit)
	{
		Vector2 firePos = mTransform->GetCenterPos() + mUnit->GetAimDirection() * (mImageInfo.image->GetFrameSize().x * 0.3f);
		_D2DRenderer->DrawLine(firePos, endPos, D2DRenderer::DefaultBrush::Red, true, 2.f);
	}
	
}

void AWP::EnterInstallation()
{
	_SoundManager->Play("cocked");
}

void AWP::Execute()
{
	if (mIsFire == false)
	{
		Vector2 direction = mUnit->GetAimDirection();
		Vector2 firePos = mTransform->GetCenterPos() + direction * (mImageInfo.image->GetFrameSize().x * 0.3f);
		float range = Vector2::Length(&(endPos - firePos));
		Figure::Line line(firePos, endPos);
		const vector<GameObject*>* list = _World->GetRenderPool()->GetObjectList(RenderPool::Layer::Monster);
		for (UINT i = 0; i < list->size(); ++i)
		{
			if (list->at(i)->GetActive())
			{
				Monster* monster = reinterpret_cast<Monster*>(list->at(i));
				if (Vector2::Length(&(firePos - monster->GetTransform()->GetCenterPos())) < range + 100.f)
				{
					if (Figure::IntersectLineToRect(nullptr, line, monster->GetCollisionRect()))
					{
						monster->Damage(5.f, direction);
					}
				}
			}
		}

		mPointLight->SetActive(true);
		mPointLight->SetRange(_startGunFireRange);
		mPointLight->SetIntensity(_startGunFireIntensity);
		mPointLight->GetTransform()->SetWorldPosition(firePos);

		mIsFire = true;
		mCurrentDelay = 0.f;
		mEffecter->RequestPlayEffect("Smoke0", 0.07f, firePos, 1.f, FrameEffecter::Option::ScaleAlphablending);
		_Camera->Shake(0.5f, 0.03f, 5.f);
		_SoundManager->Play("AWPShot");
	}
}

void AWP::ExitInstallation()
{
	_SoundManager->Play("cocked");
}
