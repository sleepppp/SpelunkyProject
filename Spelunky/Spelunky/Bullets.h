#pragma once
#include "Bullet.h"
#include "GameObject.h"
#include "RePlayDatas.h"
template<typename T> 
class Bullets : public GameObject
{
private:
	typedef typename list<Bullet*>::iterator BulletIter;
private:
	UINT mCapacity;
	list<Bullet*> mActiveBullets;
	list<Bullet*> mDeActiveBullets;

	struct SaveInfo
	{
		UINT count;
		vector<Bullet> activeBulletInfo;
	};
	RePlayDatas<SaveInfo>* mRePlayDatas;
public:
	Bullets(const UINT& capacity = 50)
		:GameObject("Bullets"),mCapacity(capacity) 
	{
		mLayer = RenderPool::Layer::Bullet;
		for (UINT i = 0; i < capacity; ++i)
		{
			Bullet* newBullet = new T;
			mDeActiveBullets.push_back(newBullet);
		}
		mRePlayDatas = new RePlayDatas<SaveInfo>();
	}
	virtual ~Bullets()
	{
		SafeDelete(mRePlayDatas);
		BulletIter iter = mActiveBullets.begin();
		for (; iter != mActiveBullets.end(); ++iter)
			SafeDelete(*iter);
		iter = mDeActiveBullets.begin();
		for (; iter != mDeActiveBullets.end(); ++iter)
			SafeDelete(*iter);
	}

	void Init()override
	{
		BulletIter iter = mDeActiveBullets.begin();
		for (; iter != mDeActiveBullets.end(); ++iter)
			(*iter)->Init();

		GameObject* world = _World->GetObjectPool()->FindObject("World");
		if (world)
			world->GetTransform()->AddChild(mTransform);

		_World->GetUpdatePool()->RequestUpdate(this);
		_World->GetRenderPool()->RequestRender(RenderPool::Layer::Bullet, this);
	}

	void Release()override
	{
		_World->GetRenderPool()->RemoveRender(RenderPool::Layer::Bullet, this);
		_World->GetUpdatePool()->RemoveUpdate(this);

		BulletIter iter = mActiveBullets.begin();
		for (; iter != mActiveBullets.end(); ++iter)
			SafeDelete(*iter);
		iter = mDeActiveBullets.begin();
		for (; iter != mDeActiveBullets.end(); ++iter)
			SafeDelete(*iter);
	}

	void Update()override
	{
		BulletIter iter = mActiveBullets.begin();
		for (; iter != mActiveBullets.end();)
		{
			if ((*iter)->Update())
			{
				BulletIter tempIter = iter; 
				++tempIter;
				
				mDeActiveBullets.push_back(*iter);
				mActiveBullets.erase(iter);
				iter = tempIter;
			}
			else ++iter;
		}

		if (RePlayManager::GetIsPlay())
		{
			if (RePlayManager::GetNowFrame() % 100 == 0)
			{
				SaveInfo info;
				info.count = mActiveBullets.size();
				info.activeBulletInfo.assign(info.count, Bullet());
				BulletIter iter = mActiveBullets.begin();
				for (UINT i = 0; iter != mActiveBullets.end();++iter,++i)
				{
					info.activeBulletInfo[i] = **iter;
				}
				mRePlayDatas->UpdateInfo(info);
			}
		}
	}

	void Render()override
	{
		BulletIter iter = mActiveBullets.begin();
		for (; iter != mActiveBullets.end(); ++iter)
			(*iter)->Render();
	}
public:
	void Fire(const Vector2& pos, const Vector2& direction)
	{
		Bullet* bullet = this->GetDeActiveBullet();
		if (bullet)
		{
			bullet->Fire(pos, direction);
			mActiveBullets.push_back(bullet);
		}
	}

	void InitAllBullet(class Image* pImage, const float& damage, const float& speed, const Bullet::TargetType& type)
	{
		BulletIter iter = mDeActiveBullets.begin();
		for (; iter != mDeActiveBullets.end(); ++iter)
		{
			(*iter)->SetImage(pImage);
			(*iter)->SetDamage(damage);
			(*iter)->SetSpeed(speed);
			(*iter)->SetTargetType(type);
		}
		iter = mActiveBullets.begin();
		for (; iter != mActiveBullets.end(); ++iter)
		{
			(*iter)->SetImage(pImage);
			(*iter)->SetDamage(damage);
			(*iter)->SetSpeed(speed);
			(*iter)->SetTargetType(type);
		}
	}
	
	void SetTargetType(const Bullet::TargetType& type)
	{
		BulletIter iter = mDeActiveBullets.begin();
		for (; iter != mDeActiveBullets.end(); ++iter)
			(*iter)->SetTargetType(type);
		iter = mActiveBullets.begin();
		for (; iter != mActiveBullets.end(); ++iter)
			(*iter)->SetTargetType(type);
	}
	
	void SetDamage(const float& damage)
	{
		BulletIter iter = mDeActiveBullets.begin();
		for (; iter != mDeActiveBullets.end(); ++iter)
			(*iter)->SetDamage(damage);
		iter = mActiveBullets.begin();
		for (; iter != mActiveBullets.end(); ++iter)
			(*iter)->SetDamage(damage);
	}
	void SetSpeed(const float& speed)
	{
		BulletIter iter = mDeActiveBullets.begin();
		for (; iter != mDeActiveBullets.end(); ++iter)
			(*iter)->SetSpeed(speed);
		iter = mActiveBullets.begin();
		for (; iter != mActiveBullets.end(); ++iter)
			(*iter)->SetSpeed(speed);
	}
	void SetImage(class Image* image)
	{
		BulletIter iter = mDeActiveBullets.begin();
		for (; iter != mDeActiveBullets.end(); ++iter)
			(*iter)->SetImage(image);
		iter = mActiveBullets.begin();
		for (; iter != mActiveBullets.end(); ++iter)
			(*iter)->SetImage(image);
	}

	void SetSize(const Vector2& size)
	{
		BulletIter iter = mDeActiveBullets.begin();
		for (; iter != mDeActiveBullets.end(); ++iter)
			(*iter)->SetSize(size);
		iter = mActiveBullets.begin();
		for (; iter != mActiveBullets.end(); ++iter)
			(*iter)->SetSize(size);
	}

private:
	Bullet* GetDeActiveBullet()
	{
		if (mDeActiveBullets.empty())
			return nullptr; 

		Bullet* result = mDeActiveBullets.back();
		mDeActiveBullets.pop_back();
		return result;
	}

	void Reset()
	{	
		BulletIter iter = mActiveBullets.begin();
		for (; iter != mActiveBullets.end(); ++iter)
			mDeActiveBullets.push_back(*iter);
		mActiveBullets.clear();
	}

	void LoadRePlayData(const UINT64& frame)override
	{
		SaveInfo info;
		if (mRePlayDatas->GetData(frame, &info))
		{
			this->Reset();
			for (UINT i = 0; i < info.activeBulletInfo.size(); ++i)
			{
				Bullet* bullet = GetDeActiveBullet();
				*bullet = info.activeBulletInfo[i];
				mActiveBullets.push_back(bullet);
			}
		}
	}
};

