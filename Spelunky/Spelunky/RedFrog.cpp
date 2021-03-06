#include "stdafx.h"
#include "RedFrog.h"

#include "Transform.h"
#include "ParticleSystemPool.h"
#include "Rigidbody.h"
#include "FrameEffecter.h"
#include "DataContext.h"
#include "Player.h"
#include "Monster.h"
#include "Tile.h"
#include "TileManager.h"
#include "BombPool.h"
#include "DamageFont.h"

RedFrog::RedFrog(class Tile* pTile)
	:Frog(pTile)
{
	mName = "RedFrog";
	mDamage = 2;
	mFullHp = mHp = 100;
	this->AddCallbackMessage("Death", [this](TagMessage message) 
	{
		this->ExecuteDie();
		this->mIsActive = false;
		Vector2 worldPos = this->GetTransform()->GetWorldPosition();
		this->GetParticlePool()->PlayParticle("BloodRubble", worldPos);
		_SoundManager->Play("rubble", _Camera->GetDistanceFactor(worldPos));
	});
}


RedFrog::~RedFrog()
{
}

void RedFrog::Init()
{
	Frog::Init();
	mEffecter = (FrameEffecter*)_World->GetObjectPool()->FindObject("FrameEffecter");
	mBombPool = (BombPool*)_World->GetObjectPool()->FindObject("BombPool");
}


void RedFrog::CreateState()
{
	FrogIdleState* idle = new FrogIdleState(this);
	mStateManager.AddState("Idle", idle);

	FrogJumpState* jump = new FrogJumpState(this);
	mStateManager.AddState("MoveToPlayer", jump);

}


void RedFrog::CreateAnimation()
{
	constexpr float defaultUpdateTime = 0.1f;

	Animation* idle = new Animation;
	idle->SetStartEndFrame(5, 0, 8, 0, true);
	idle->SetIsLoop(true);
	idle->SetFrameUpdateTime(defaultUpdateTime);
	mAnimations.AddAnimation("Idle", idle);

	Animation* jump = new Animation;
	jump->SetVectorFrame({ {9,0} });
	jump->SetIsLoop(false);
	jump->SetFrameUpdateTime(defaultUpdateTime);
	mAnimations.AddAnimation("MoveToPlayer", jump);
}

void RedFrog::Damage(const int & damage, const Vector2 & forceDirection, const float & forcePower, const float & recuPower)
{
	if (mHp > 0)
	{
		mHp -= damage;
		if (mHp <= 0)
		{
			this->mIsActive = false;
			TileManager* tileManager =reinterpret_cast<TileManager*>
				(_World->GetRenderPool()->FindObjectInLayer(RenderPool::Layer::Tile, "TileManager"));
			
			float tileSize = Tile::GetTileSize();
			Vector2 pos = mTransform->GetWorldPosition() - Vector2(0.f, tileSize / 2.f);
			int indexX = CastingInt(pos.x / tileSize);
			int indexY = CastingInt(pos.y / tileSize);
			
			for (int y = indexY - 1; y <= indexY + 1; ++y)
			{
				for (int x = indexX - 1; x <= indexX + 1; ++x)
				{
					if (Tile* tile = tileManager->GetTile(x, y))
					{
						tile->Explosion();
					}
				}
			}
			
			const vector<GameObject*>* list = _World->GetRenderPool()->GetObjectList(RenderPool::Layer::Monster);
			for (UINT i = 0; i < list->size(); ++i)
			{
				GameObject* target = list->at(i);
				if (target->GetActive() == true)
				{
					Vector2 direction = target->GetTransform()->GetWorldPosition() - pos;
					float distance = Vector2::Length(&direction);
					if (distance < tileSize * 2.f)
					{
						if (Monster* monster = dynamic_cast<Monster*>(target))
						{
							if(monster != this)
								monster->Damage(50, direction, 1000, 1500.f);
						}
					}
				}
			}
			GameObject* player = _World->GetRenderPool()->FindObjectInLayer(RenderPool::Layer::Character, "Player");
			Vector2 direction = player->GetTransform()->GetWorldPosition() - pos;
			float distance = Vector2::Length(&(player->GetTransform()->GetWorldPosition() - pos));
			if (distance < tileSize * 2.f)
			{
				if (Unit* unit = dynamic_cast<Unit*>(player))
				{
					unit->Damage(2, direction, 1700.f, 2000.f);
				}
			}
			
			mBombPool->ActivationLight(pos);
			
			float factor = _Camera->GetDistanceFactor(pos);
			
			float shakeTime = 1.f * factor;
			float shakeChangeDirTime = 0.03f * factor;
			float shakePower = 10.4f * factor;
			_Camera->Shake(shakeTime, shakeChangeDirTime, shakePower);
			_SoundManager->Play("kaboom", factor);
			_SoundManager->Play("FrogHouling", _Camera->GetDistanceFactor(mTransform->GetWorldPosition()* 1.4f));
			Vector2 worldPos = this->GetTransform()->GetWorldPosition();
			this->GetParticlePool()->PlayParticle("BloodRubble", worldPos);
			_SoundManager->Play("rubble", _Camera->GetDistanceFactor(worldPos));
			mEffecter->RequestPlayEffect("SmokeOrange", 0.1f, mTransform->GetCenterPos(), 1.f, FrameEffecter::Option::ScaleAlphablending);
			
			DataContextValue* value = _GameData->GetData(GameData::DataType::Int, "KillingMonsterCount");
			_GameData->SetData(GameData::DataType::Int, "KillingMonsterCount", value->GetInt() + 1);
			mDamageFont->RequestDamageFont(to_wstring(damage), mTransform->GetCenterPos(), 230.f, 45, GameColor(1.f, 0.f, 0.f, 1.f));
		}
		else
		{
			mParticlePool->PlayParticle("BloodRubble", mTransform->GetWorldPosition());
			mRigidbody->Force(forceDirection, forcePower, recuPower);
			mDamageFont->RequestDamageFont(to_wstring(damage), mTransform->GetCenterPos());
			_SoundManager->Play("FrogHouling", _Camera->GetDistanceFactor(mTransform->GetWorldPosition()* 1.4f));
		}
	}
}

