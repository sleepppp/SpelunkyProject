/*************************************************************
## CameraManager ##
카메라 싱글톤
*************************************************************/
#pragma once
#include "SingletonBase.h"

class CameraManager final : public SingletonBase<CameraManager>
{
	BlockAssign(CameraManager)
private:
	static const float _moveStartDistance;
	static const float _maxSpeed;
	static const float _minSpeed;
	static const float _maxDistance;
	static const float _minDistance; 
	static const float _zoomMax; 
	static const float _zoomMin; 
public:
	enum MoveState { None, MoveToTarget, FreeCamera, End };		//카메라 상태 값
private:
	MoveState mState;
	Figure::FloatRect mRect; 
	Vector2 mPosition;
	Vector2 mMapSize; 
	Vector2 mSaveMouse;
	float mZoomFactor; 
private:
	friend class SingletonBase<CameraManager>;
	CameraManager();
	virtual ~CameraManager();
public:
	void Update();

	Vector2 GetPosition()const { return mPosition; }
	Vector2 GetMapSize()const { return mMapSize; }
	float GetZoom()const { return mZoomFactor; }
	Figure::FloatRect GetRect()const { return mRect; }

	Figure::FloatRect GetRelativeRect(const Figure::FloatRect& rc);
	Vector2 GetRelativeVector2(const Vector2& v);

	void GetRelativeRect(Figure::FloatRect*const pOutput);
	void GetRelativeVector2(Vector2*const pOutput);
	void GetWorldVector2(Vector2*const pOutput);
	Vector2 GetWorldMouse();
private:
	void UpdateRenderRect();
	void UpdateFreeCameraMode();
	void AmendCamera();
private:
	friend class Window;
	void CameraProc(WPARAM wParam) ;
};

#define _Camera CameraManager::Get()