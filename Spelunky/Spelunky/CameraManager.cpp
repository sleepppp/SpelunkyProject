#include "stdafx.h"
#include "CameraManager.h"

#include "GameObject.h"
#include "Transform.h"

const float CameraManager::_moveStartDistance = 30.f;
const float CameraManager::_maxSpeed = 400.f;
const float CameraManager::_minSpeed = 100.f;
const float CameraManager::_maxDistance = 250.f; 
const float CameraManager::_minDistance = 30.f;
const float CameraManager::_zoomMax = 2.5f;
const float CameraManager::_zoomMin = 0.1f; 

CameraManager::CameraManager()
	:mZoomFactor(1.f),mState(CameraManager::MoveState::FreeCamera)
{
	mSaveMouse = _Input->GetMousePosition();
}

CameraManager::~CameraManager() {}

void CameraManager::Update()
{
	switch (mState)
	{
	case CameraManager::None:
	case CameraManager::MoveToTarget:
		break;
	case CameraManager::FreeCamera:
		this->UpdateFreeCameraMode();
		break;
	}
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
	Vector2 result; 
	this->GetWorldVector2(&result);
	return result;
}


void CameraManager::CameraProc(WPARAM wParam)
{
	if (mState == MoveState::FreeCamera)
	{
		if (IsMouseOnGui == false)
		{
			if ((SHORT)HIWORD(wParam) > 0)
			{
				this->mZoomFactor += 0.1f;
				this->mZoomFactor = Math::Clampf(mZoomFactor, _zoomMin, _zoomMax);
			}
			else
			{
				this->mZoomFactor -= 0.1f;
				this->mZoomFactor = Math::Clampf(mZoomFactor, _zoomMin, _zoomMax);
			}
		}
	}
}

void CameraManager::UpdateRenderRect()
{
	mRect.Update(mPosition, Vector2(_WinSizeX, _WinSizeY), Pivot::Center);
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

void CameraManager::AmendCamera()
{
	if (mRect.left < 0.f)
	{
		mPosition.x -= mPosition.x;
		mRect.Update(mPosition, Vector2(_WinSizeX, _WinSizeY), Pivot::Center);
	}
	else if (mRect.right > mMapSize.x)
	{
		mPosition.x -= ((float)mRect.right) - mMapSize.x;
		mRect.Update(mPosition, Vector2(_WinSizeX, _WinSizeY), Pivot::Center);
	}
	if (mRect.top < 0.f)
	{
		mPosition.y -= mPosition.y;
		mRect.Update(mPosition, Vector2(_WinSizeX, _WinSizeY), Pivot::Center);
	}
	else if (mRect.bottom > mMapSize.y)
	{
		mPosition.y -= (float(mRect.bottom)) - mMapSize.y;
		mRect.Update(mPosition, Vector2(_WinSizeX, _WinSizeY), Pivot::Center);
	}
}
