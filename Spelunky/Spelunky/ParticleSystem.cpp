#include "stdafx.h"
#include "ParticleSystem.h"
#include "Transform.h"
#include "Particle.h"
#include "BinaryFile.h"
#include "Path.h"
#include "StringHelper.h"
ParticleSystem::ParticleSystem(const UINT& capacity)
	:GameObject("ParticleSystem"),mCapacity(capacity), mIsDuration(true)
{
	mLayer = RenderPool::Layer::Effect;
	mIsActive = true;
	for (UINT i = 0; i < mCapacity; ++i)
		mDeActiveParticles.push_back(new Particle);
	mMainOption.mCurrentTime = mMainOption.mCurrentDelayTime = 0.f;
}

ParticleSystem::ParticleSystem(const wstring & filePath)
	:GameObject(StringHelper::WStringToString(filePath)), mCapacity(0), mIsDuration(false)
{

	mLayer = RenderPool::Layer::Effect;
	this->LoadData(filePath);
	mMainOption.mCurrentTime = mMainOption.mCurrentDelayTime = 0.f;
}

ParticleSystem::~ParticleSystem()
{
	ParticleIter iter = mActiveParticles.begin();
	for (; iter != mActiveParticles.end(); ++iter)
		SafeDelete(*iter);
	mActiveParticles.clear();
	iter = mDeActiveParticles.begin();
	for (; iter != mDeActiveParticles.end(); ++iter)
		SafeDelete(*iter);
	mDeActiveParticles.clear();
}

void ParticleSystem::Init()
{
	ParticleIter iter = mDeActiveParticles.begin();
	for (; iter != mDeActiveParticles.end(); ++iter)
		(*iter)->Init();
	_World->GetUpdatePool()->RequestUpdate(this);
	_World->GetRenderPool()->RequestRender(mLayer, this);

	GameObject* world = _World->GetObjectPool()->FindObject("World");
	if(world)
		world->GetTransform()->AddChild(mTransform);
}

void ParticleSystem::Update()
{
	const float deltaTime = _TimeManager->DeltaTime();
	if (mIsDuration)
	{
		//시간제어 변수들 계산
		mMainOption.mCurrentTime += deltaTime;
		if (mMainOption.mCurrentTime >= mMainOption.mDurationTime)
		{
			if(mMainOption.mIsLoop == false)
				this->Stop();
			else
			{
				while (mMainOption.mCurrentTime >= mMainOption.mDurationTime)
					mMainOption.mCurrentTime -= mMainOption.mDurationTime;
			}
		}
		mMainOption.mCurrentDelayTime += deltaTime;
		//방사 시간 체크
		if (mMainOption.mCurrentDelayTime >= mMainOption.mDurationDelayTime)
		{
			while (mMainOption.mCurrentDelayTime >= mMainOption.mDurationDelayTime)
				mMainOption.mCurrentDelayTime -= mMainOption.mDurationDelayTime;
			//파티클들을 방사해라 

			//한번에 방사할 파티클 수만큼 방사 
			for (int i = 0; i < mMainOption.mDurationCount; ++i)
			{
				//방사가능 한 파티클이 없다면 생략 
				if (mDeActiveParticles.empty())
					break; 
				Particle* particle = mDeActiveParticles.back();
				mDeActiveParticles.pop_back();
				float liveTime = mMainOption.mParticleLiveTime;
				float speed = mMainOption.mParticleSpeed;
				float speedAccelation = 0.f;
				float angleAccelation = 0.f;
				Vector2 startPos = mTransform->GetWorldPosition();
				Vector2 startSize = mMainOption.mParticleStartSize;
				Vector2 startDirection;
				//원형 이라면
				if (mShapeOption.mShape == ParticleShapeOption::Shape::Circle)
				{
					startDirection.x = Math::Random(-1.f, 1.f);
					startDirection.y = Math::Random(-1.f, 1.f);
					Vector2::Normalize(&startDirection);

					float randomOffsetX = Math::Random(-mShapeOption.mRadius, mShapeOption.mRadius);
					float randomOffsetY = Math::Random(-mShapeOption.mRadius, mShapeOption.mRadius);
					startPos.x += randomOffsetX;
					startPos.y += randomOffsetY;
				}
				//콘 모양이라면
				else if(mShapeOption.mShape == ParticleShapeOption::Shape::Corn)
				{
					float angle = mShapeOption.mParticleStartDirection;
					angle = Math::Random((angle - mShapeOption.mDirectionOffset), (angle + mShapeOption.mDirectionOffset));
					startDirection.x = cosf(angle);
					startDirection.y = -sinf(angle);
					Vector2::Normalize(&startDirection);
				}
				
				//랜덤 값 검사 
				if (mRandomOption.mUseRandomLiveTime)
					liveTime = Math::Random(mRandomOption.mMinLiveTime, mRandomOption.mMaxLiveTime);
				if (mRandomOption.mUseRandomSpeed)
					speed = Math::Random(mRandomOption.mMinSpeed, mRandomOption.mMaxSpeed);
				if (mRandomOption.mUseRandomSize)
				{
					startSize.x = Math::Random(mRandomOption.mMinSize.x, mRandomOption.mMaxSize.x);
					startSize.y = Math::Random(mRandomOption.mMinSize.y, mRandomOption.mMaxSize.y);
				}
				if (mRandomOption.mUseRandomSpeedAccelation)
				{
					speedAccelation = Math::Random(mRandomOption.mMinSpeedAccelation, mRandomOption.mMaxSpeedAccelation);
				}
				if (mRandomOption.mUseRandomAngleAccelation)
				{
					angleAccelation = Math::Random(mRandomOption.mMinAngleAccelation, mRandomOption.mMaxAngleAccelation);
				}
				int randomIndex = Math::Random(0, mRenderOption.mImageList[mRenderOption.mRubbleType].size() -1 );
				ImageInfo info = mRenderOption.mImageList[mRenderOption.mRubbleType][randomIndex];
				particle->SetImageInfo(info.image,info.frameX,info.frameY);
				particle->SetTimeInfo(liveTime);
				particle->SetPhysicsInfo(startPos, startSize, startDirection, speed, Math::Random(0.f,360.f));
				particle->SetUseGravity(mShapeOption.mUsePhysics, mShapeOption.mMass);
				particle->SetRenderType(mRenderOption.mRenderType,mRenderOption.mColor);
				particle->SetInterpolateInfo(speedAccelation, angleAccelation, Vector2());
				particle->SetRelativeCamera(mMainOption.mIsRelative);
				mActiveParticles.push_back(particle);
			}

		}
	}

	ParticleIter iter = mActiveParticles.begin();
	for (; iter != mActiveParticles.end(); )
	{
		if ((*iter)->Update(deltaTime))
		{
			ParticleIter tempIter = iter;
			++tempIter;
			mDeActiveParticles.push_back((*iter));
			mActiveParticles.erase(iter);
			iter = tempIter;
		}
		else
			++iter;
	}
}

void ParticleSystem::Render()
{
	ParticleIter iter = mActiveParticles.begin();
	for (; iter != mActiveParticles.end(); ++iter)
		(*iter)->Render();

	//if (_isDebug)
	//{
	//	if(mShapeOption.mShape == ParticleShapeOption::Shape::Circle)
	//		_D2DRenderer->DrawEllipse(mTransform->GetWorldPosition(), mShapeOption.mRadius, D2DRenderer::DefaultBrush::Red,
	//			mMainOption.mIsRelative, 2.f);
	//	else if (mShapeOption.mShape == ParticleShapeOption::Shape::Corn)
	//	{
	//		Vector2 origin = mTransform->GetWorldPosition();
	//		float angle = mShapeOption.mParticleStartDirection;
	//		Vector2 minAngle, maxAngle;
	//		minAngle.x = cosf(angle - mShapeOption.mDirectionOffset);
	//		minAngle.y = -sinf(angle - mShapeOption.mDirectionOffset);
	//		maxAngle.x = cosf(angle + mShapeOption.mDirectionOffset);
	//		maxAngle.y = -sinf(angle + mShapeOption.mDirectionOffset);
	//
	//		Vector2 endPoint0 = origin + Vector2(minAngle.x * mShapeOption.mRadius,minAngle.y * mShapeOption.mRadius);
	//		Vector2 endPoint1 = origin + Vector2(maxAngle.x * mShapeOption.mRadius, maxAngle.y * mShapeOption.mRadius);
	//		_D2DRenderer->DrawLine(origin, endPoint0, D2DRenderer::DefaultBrush::Red, mMainOption.mIsRelative, 2.f);
	//		_D2DRenderer->DrawLine(origin, endPoint1, D2DRenderer::DefaultBrush::Red, mMainOption.mIsRelative, 2.f);
	//		_D2DRenderer->DrawLine(endPoint0, endPoint1, D2DRenderer::DefaultBrush::Red, mMainOption.mIsRelative, 2.f);
	//	}
	//	this->OnGui();
	//}
}

void ParticleSystem::OnGui()
{
	if (_isDebug)
	{
		ImGui::Begin(mName.c_str());
		if (ImGui::Button("Save"))
			this->SaveData();
		ImGui::SameLine();
		if (ImGui::Button("Load"))
			this->LoadData();
		if (ImGui::Checkbox("Duration", &mIsDuration))
		{
			if (mIsDuration)this->Play();
			else this->Stop();
		}
		ImGui::Text("Capacity : %d", mCapacity);
		ImGui::Text("ActiveCount : %d", (int)mActiveParticles.size());
		ImGui::Separator();
		mMainOption.OnGui();
		mShapeOption.OnGui();
		mRandomOption.OnGui();
		mRenderOption.OnGui();
		ImGui::End();
	}
}

void ParticleSystem::Play()
{
	mIsDuration = true;
	mIsActive = true; 
}

void ParticleSystem::Pause()
{
	mIsDuration = false;
}

void ParticleSystem::Stop()
{
	mIsDuration = false;
	mMainOption.mCurrentTime = mMainOption.mCurrentDelayTime = 0.f;
}

bool ParticleSystem::IsTimeToSleep()
{
	if (mIsDuration == false && mActiveParticles.size() == 0)
		return true;
	return false;
}

void ParticleSystem::SaveData(const wstring & filePath)
{
	if (filePath.length() > 0)
	{
		BinaryWriter* w = new BinaryWriter;
		w->Open(filePath);
		{
			w->UInt(mCapacity);
			mMainOption.SaveData(w);
			mShapeOption.SaveData(w);
			mRandomOption.SaveData(w);
			mRenderOption.SaveData(w);
		}
		w->Close();
		SafeDelete(w);
	}
	else
	{
		function<void(wstring)> func = bind(&ParticleSystem::SaveData, this, placeholders::_1);
		Path::SaveFileDialog(filePath, nullptr, L"../GameData/ParticleSystem/", func);
	}
}

void ParticleSystem::LoadData(const wstring & filePath)
{
	if (filePath.length() > 0)
	{
		BinaryReader* r = new BinaryReader;
		r->Open(filePath);
		{
			mCapacity = r->UInt();
			mMainOption.LoadData(r);
			mShapeOption.LoadData(r);
			mRandomOption.LoadData(r);
			mRenderOption.LoadData(r);
		}
		r->Close();
		SafeDelete(r);

		if (mDeActiveParticles.size() + mActiveParticles.size() < mCapacity)
		{
			UINT newParticleSize = mCapacity - mDeActiveParticles.size() + mActiveParticles.size();
			for (UINT i = 0; i < newParticleSize; ++i)
				mDeActiveParticles.push_back(new Particle);
		}
	}
	else
	{
		function<void(wstring)> func = bind(&ParticleSystem::LoadData, this, placeholders::_1);
		Path::OpenFileDialog(L"", nullptr, L"../GameData/ParticleSystem/", func);
	}
}
