/*************************************************************
## CameraManager ##
ī�޶� �̱���
*************************************************************/
#pragma once
#include "SingletonBase.h"

class CameraManager final : public SingletonBase<CameraManager>
{
	BlockAssign(CameraManager)
private:
	static const float _zoomMax; 
	static const float _zoomMin; 

	static const float _minSpeed; 
	static const float _maxSpeed; 
	static const float _minDistance;
	static const float _maxDistance;
public:
	enum MoveState { Target, MoveToTarget, FreeCamera, End };		//ī�޶� ���� ��
private:
	MoveState mState;
	Figure::FloatRect mRect; 
	Vector2 mPosition;
	Vector2 mMapSize; 
	Vector2 mSaveMouse;
	float mZoomFactor; 
	class Transform* mTarget;

	bool mIsShake;
	Vector2 mShakePosition;
	Vector2 mShakeDir;
	float mShakeStrength;
	float mShakeTime; 
	float mShakeChangeDirDelay;
	float mShakeCurrentTime;
	float mShakeChangeDirCurrentTime;
private:
	friend class SingletonBase<CameraManager>;
	CameraManager();
	virtual ~CameraManager();
public:
	void InitCamera();
	void Update();
	void OnGui();

	inline Vector2 GetPosition()const { return mPosition; }
	inline Vector2 GetMapSize()const { return mMapSize; }
	inline float GetZoom()const { return mZoomFactor; }
	inline Figure::FloatRect GetRect()const { return mRect; }
	void SetMapSize(const Vector2& v) { mMapSize = v; }

	Figure::FloatRect GetRelativeRect(const Figure::FloatRect& rc);
	Vector2 GetRelativeVector2(const Vector2& v);

	void GetRelativeRect(Figure::FloatRect*const pOutput);
	void GetRelativeVector2(Vector2*const pOutput);
	void GetWorldVector2(Vector2*const pOutput);
	Vector2 GetWorldMouse();

	void AddZoom(const float& factor);
	void SetZoom(const float& zoom) { mZoomFactor = zoom; }
	void Move(const Vector2& moveValue);
	bool IsInCamera(const Figure::FloatRect& rc);

	void SetTarget(class Transform* pTransform)
	{
		mTarget = pTransform;
		mState = CameraManager::MoveState::Target;
	}
	void ChangeTarget(class Transform* pTransform)
	{
		mTarget = pTransform;
		mState = CameraManager::MoveState::MoveToTarget;
	}

	float GetDistanceFactor(const Vector2& point);

	void Shake(const float& shakeTime, const float& changeDirTime, const float& shakeStrength);
private:
	void UpdateRenderRect();
	void UpdateFreeCameraMode();
	void UpdateTargetCameraMode();
	void UpdateMoveToTargetCameraMode();
	void UpdateShake();
	void AmendCamera();
private:
	friend class Window;
	void CameraProc(WPARAM wParam) ;
};

#define _Camera CameraManager::Get()