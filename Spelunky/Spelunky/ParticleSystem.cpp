#include "stdafx.h"
#include "ParticleSystem.h"
#include "Transform.h"
#include "Particle.h"
ParticleSystem::ParticleSystem(const UINT& capacity)
	:GameObject("ParticleSystem"),mCapacity(capacity), mIsDuration(true)
{
	mLayer = RenderPool::Layer::Effect;
	mIsActive = true;
	for (UINT i = 0; i < mCapacity; ++i)
		mDeActiveParticles.push_back(new Particle);
}

ParticleSystem::ParticleSystem(const wstring & filePath)
{
	mLayer = RenderPool::Layer::Effect;
	this->LoadData(filePath);
	for (UINT i = 0; i < mCapacity; ++i)
		mDeActiveParticles.push_back(new Particle);
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
	_World->GetUpdatePool()->RequestUpdate(this);
	_World->GetRenderPool()->RequestRender(mLayer, this);
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
				
				particle->SetTimeInfo(liveTime);
				particle->SetPhysicsInfo(startPos, startSize, startDirection, speed, Math::Random(0.f,360.f), false);
				particle->SetRenderType(mRenderOption.mRenderType,mRenderOption.mColor);
				particle->SetInterpolateInfo(speedAccelation, angleAccelation, Vector2());
				
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

	if (_isDebug)
	{
		if(mShapeOption.mShape == ParticleShapeOption::Shape::Circle)
			_D2DRenderer->DrawEllipse(mTransform->GetWorldPosition(), mShapeOption.mRadius, D2DRenderer::DefaultBrush::Red,
				true, 2.f);
		else if (mShapeOption.mShape == ParticleShapeOption::Shape::Corn)
		{
			Vector2 origin = mTransform->GetWorldPosition();
			float angle = mShapeOption.mParticleStartDirection;
			Vector2 minAngle, maxAngle;
			minAngle.x = cosf(angle - mShapeOption.mDirectionOffset);
			minAngle.y = -sinf(angle - mShapeOption.mDirectionOffset);
			maxAngle.x = cosf(angle + mShapeOption.mDirectionOffset);
			maxAngle.y = -sinf(angle + mShapeOption.mDirectionOffset);

			Vector2 endPoint0 = origin + Vector2(minAngle.x * mShapeOption.mRadius,minAngle.y * mShapeOption.mRadius);
			Vector2 endPoint1 = origin + Vector2(maxAngle.x * mShapeOption.mRadius, maxAngle.y * mShapeOption.mRadius);
			_D2DRenderer->DrawLine(origin, endPoint0, D2DRenderer::DefaultBrush::Red, true, 2.f);
			_D2DRenderer->DrawLine(origin, endPoint1, D2DRenderer::DefaultBrush::Red, true, 2.f);
			_D2DRenderer->DrawLine(endPoint0, endPoint1, D2DRenderer::DefaultBrush::Red, true, 2.f);
		}
	}

	this->OnGui();
}

void ParticleSystem::OnGui()
{
	if (_isDebug)
	{
		ImGui::Begin(mName.c_str());
		ImGui::Checkbox("Duration", &mIsDuration);
		ImGui::Text("Capacity : %d", mCapacity);
		ImGui::Text("ActiveCount : %d", (int)mActiveParticles.size());
		ImGui::Separator();
		mMainOption.OnGui();
		mShapeOption.OnGui();
		mRandomOption.OnGui();
		mRenderOption.OnGui();
		mInterpolateOption.OnGui();
		ImGui::End();
	}
}

void ParticleSystem::Play()
{
	mIsDuration = true;
}

void ParticleSystem::Pause()
{
	mIsDuration = false;
}

void ParticleSystem::Stop()
{
	mIsDuration = false;
}

void ParticleSystem::SaveData(const wstring & filePath)
{
}

void ParticleSystem::LoadData(const wstring & filePath)
{
}
