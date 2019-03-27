/*************************************************************
## CameraManager ##
카메라 싱글톤
*************************************************************/
#pragma once
#include "SingletonBase.h"
#define CameraMoveStartDistance 30.f		

#define CameraMaxSpeed 400.0f			//카메라 최대 스피드
#define CameraMinSpeed 100.0f			//카메라 최소 스피드
#define CameraMaxDistance 250.f			
#define CameraMinDistance 30.f 

#define CameraZoomMax 2.5f				//카메라 줌 최대값
#define CameraZoomMin 0.5f				//카메라 줌 최소값

#define CameraDefaultStrength 2.58f
#define CameraDefaultShakeTime 0.2f
#define CameraDefaultChangeDirectionDelay 0.043f

class CameraManager : public SingletonBase<CameraManager>
{
	BlockAssign(CameraManager)
public:
	enum MoveState { None, MoveToTarget, FreeCamera, End };		//카메라 상태 값
private:
	MoveState state;								//카메라 상태
	float zoomFactor;								//카메라 줌 
	Figure::FloatRect cameraRect;								//카메라 렉트
	Synthesize(Vector2, position, Position)			//카메라 좌표
	class GameObject* pTarget;						//카메라가 따라갈 GameObject(추적카메라 구현 시 사용)
	Synthesize(Vector2, mapSize, MapSize)				//전체 맵 사이즈 (카메라 보정이 들어감)
	Synthesize(float, speed, Speed)					//카메라 스피드 값
	Vector2 saveMouse;								//자유시점 카메라에 사용(이전 프레임 마우스 좌표 저장 값)

	bool isShake;
	float shakeStrength;
	float totalShakeTime;
	float shakeTime;
	float shakeDirection;
	float moveStartDistance;
	float shakeChangeDelayTime;
	float shakeChangeDirectionTime;
private:
	friend class SingletonBase<CameraManager>;
	CameraManager();
	virtual ~CameraManager();
public:
	void Update();
	void OnGui();

	Figure::FloatRect GetRelativeRect(Figure::FloatRect rc);
	POINT GetRelatviePoint(POINT pt);
	Vector2 GetRelativeVector2(Vector2 v);

	Vector2 GetWorldVector2(Vector2 v);
	const Figure::FloatRect& GetCameraRect()const { return cameraRect; }
	Vector2 GetWorldMouse();
	Vector2 GetWorldViewportCenter();
	POINT GetWorldMousePoint();
	void SetZoom(float zoom);
	float GetZoom()const { return this->zoomFactor; }
	void CameraProc(UINT message, WPARAM wParam, LPARAM lParam);
	void SetTarget(class GameObject* object);
	void ChangeTarget(class GameObject* object);
	void SetCameraMoveDistance(float distance = CameraMoveStartDistance) { moveStartDistance = distance; }

	void Shake(float strength = CameraDefaultStrength, float shakeTime = CameraDefaultShakeTime,
		float delayChangeDirection = CameraDefaultChangeDirectionDelay);
	void InitCameraToPlayer();
	void SetState(MoveState state) { this->state = state; }
	void SetFreeCamera();
private:
	void UpdateRenderRect();
	void UpdateFreeCameraMode();
	void UpdateTargetCameraMode();
	void AmendCamera();
	void ShakingUpdate();
};

#define _Camera CameraManager::Get()