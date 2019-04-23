#include "stdafx.h"
#include "CameraManager.h"

#include "GameObject.h"
#include "Transform.h"

const float CameraManager::_zoomMax = 2.5f;
const float CameraManager::_zoomMin = 0.1f; 

CameraManager::CameraManager()
	:mZoomFactor(1.f),mState(CameraManager::MoveState::FreeCamera),mIsShake(false),mShakeStrength(0.f),mShakeTime(0.f),
	mShakeChangeDirDelay(0.f),mShakeCurrentTime(0.f),mShakeChangeDirCurrentTime(0.f), mShakeDir(1.f,0.f)
{
	mSaveMouse = _Input->GetMousePosition();
	this->UpdateRenderRect();
}

CameraManager::~CameraManager() {}

void CameraManager::Update()
{
	Vector2 dir;
	Vector2 centerPos;
	switch (mState)
	{
	case CameraManager::None:
	case CameraManager::MoveToTarget:
		this->UpdateTargetCameraMode();
		break;
	case CameraManager::FreeCamera:
		this->UpdateFreeCameraMode();
		break;
	}
}

void CameraManager::OnGui()
{
	ImGui::Begin("Camera");
	ImGui::Text("Zoom : %f", mZoomFactor);
	ImGui::End();
}

Figure::FloatRect CameraManager::GetRelativeRect(const Figure::FloatRect & rc)
{
	Figure::FloatRect result = rc; 
	result.Move(Vector2(-mRect.left,-mRect.top));
	result.left *= mZoomFactor;
	result.top *= mZoomFactor;
	result.right *= mZoomFactor;
	result.bottom *= mZoomFactor;

	return result; 
}

Vector2 CameraManager::GetRelativeVector2(const Vector2 & v)
{
	Vector2 result = v;
	result -= Vector2(mRect.left, mRect.top);
	result.x *= mZoomFactor;
	result.y *= mZoomFactor;
	return result;
}

void CameraManager::GetRelativeRect(Figure::FloatRect * const pOutput)
{
	pOutput->Move(Vector2(-mRect.left,-mRect.top));
	pOutput->left *= mZoomFactor;
	pOutput->top *= mZoomFactor;
	pOutput->right *= mZoomFactor;
	pOutput->bottom *= mZoomFactor;
}

void CameraManager::GetRelativeVector2(Vector2 * const pOutput)
{
	pOutput->x -= mRect.left;
	pOutput->y -= mRect.top;
	pOutput->x *= mZoomFactor;
	pOutput->y *= mZoomFactor;
}

void CameraManager::GetWorldVector2(Vector2 * const pOutput)
{
	pOutput->x /= mZoomFactor;
	pOutput->y /= mZoomFactor;
	pOutput->x += mRect.left;
	pOutput->y += mRect.top;
}

Vector2 CameraManager::GetWorldMouse()
{
	Vector2 result = _Input->GetMousePosition(); 
	this->GetWorldVector2(&result);
	return result;
}

void CameraManager::AddZoom(const float& factor)
{
	this->mZoomFactor += factor;
	this->mZoomFactor = Math::Clampf(mZoomFactor, _zoomMin, _zoomMax);

	this->mRect.Update(mPosition, Vector2(_WinSizeX / mZoomFactor, _WinSizeY / mZoomFactor), Pivot::Center);
	this->mSaveMouse = _Input->GetMousePosition();
	if (mState != FreeCamera)
		this->AmendCamera();
}

void CameraManager::Move(const Vector2 & moveValue)
{
	this->mPosition += moveValue;
	this->UpdateRenderRect();
}

bool CameraManager::IsInCamera(const Figure::FloatRect & rc)
{
	return Figure::IntersectRectToRect(&rc, &mRect);
}


void CameraManager::CameraProc(WPARAM wParam)
{
	if (mState == MoveState::FreeCamera)
	{
		if (IsMouseOnGui == false)
		{
			if ((SHORT)HIWORD(wParam) > 0)
				this->AddZoom(0.1f);
			else
				this->AddZoom(-0.1f);
		}
	}
}

float CameraManager::GetDistanceFactor(const Vector2 & point)
{
	//가까울수록 큰 값 반환
	static const float maxDistance = sqrtf(CastingFloat(_WinSizeX * _WinSizeX + _WinSizeY * _WinSizeY));
	float length = Vector2::Length(&(mPosition - point)) + Math::Epsilon;
	return Math::Clampf(1.f - length / maxDistance,0.f,1.f);
}

void CameraManager::Shake(const float & shakeTime, const float & changeDirTime, const float & shakeStrength)
{
	mShakeChangeDirCurrentTime = 0.f;
	mShakeCurrentTime = mShakeTime = shakeTime;
	mShakeStrength = shakeStrength;
	mShakeChangeDirDelay = changeDirTime;
	mIsShake = true;
}

void CameraManager::UpdateRenderRect()
{
	mRect.Update(mPosition + mShakePosition, Vector2(CastingFloat(_WinSizeX) / mZoomFactor, CastingFloat(_WinSizeY) / mZoomFactor), Pivot::Center);
}

void CameraManager::UpdateFreeCameraMode()
{
	if (IsMouseOnGui == false)
	{
		Vector2 currentMouse = _Input->GetMousePosition();
		if (_Input->GetKey(VK_RBUTTON))
		{
			if (this->mSaveMouse != currentMouse)
			{
				this->mPosition += (mSaveMouse - currentMouse) / mZoomFactor;
				this->UpdateRenderRect();
				mSaveMouse = currentMouse;
			}
		}
		mSaveMouse = currentMouse;
	}
}

void CameraManager::UpdateTargetCameraMode()
{
	Vector2 newPos = mTarget->GetCenterPos();
	if (newPos != mPosition)
	{
		this->mPosition = mTarget->GetCenterPos();
		this->UpdateRenderRect();
		this->AmendCamera();
	}
	if (mIsShake)
	{
		const float deltaTime = _TimeManager->DeltaTime();
		mShakeCurrentTime -= deltaTime;
		if (mShakeCurrentTime < 0.f)
		{
			mIsShake = false;
			mShakePosition = Vector2(0.f, 0.f);
			this->UpdateRenderRect();
			this->AmendCamera();
		}
		else
		{
			mShakeChangeDirCurrentTime += deltaTime;
			if (mShakeChangeDirCurrentTime >= mShakeChangeDirDelay)
			{
				mShakeDir = mShakeDir * -1.f;
				while (mShakeChangeDirCurrentTime >= mShakeChangeDirDelay)
					mShakeChangeDirCurrentTime -= mShakeChangeDirDelay;
			}
			float factor = mShakeCurrentTime / mShakeTime;
			float strengh = mShakeStrength * factor;
			strengh = strengh * mShakeDir.x;
			mShakePosition += Vector2(strengh, 0.f);
			if (mShakeCurrentTime >= mShakeTime * 0.5f)
				mShakePosition.x = mShakePosition.x * 0.5f;
			mRect.Update(mPosition + mShakePosition,
				Vector2(CastingFloat(_WinSizeX) / mZoomFactor, CastingFloat(_WinSizeY) / mZoomFactor), Pivot::Center);
			this->AmendCamera();
		}
	}
}

void CameraManager::AmendCamera()
{
	if (mRect.left < 0.f)
	{
		mPosition.x -= mRect.left;
		this->UpdateRenderRect();
	}
	else if (mRect.right > mMapSize.x)
	{
		mPosition.x -= ((float)mRect.right) - mMapSize.x;
		this->UpdateRenderRect();
	}
	if (mRect.top < 0.f)
	{
		mPosition.y -= mRect.top;
		this->UpdateRenderRect();
	}
	else if (mRect.bottom > mMapSize.y)
	{
		mPosition.y -= (float(mRect.bottom)) - mMapSize.y;
		this->UpdateRenderRect();
	}
}
