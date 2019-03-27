/*************************************************************
## CameraManager ##
ī�޶� �̱���
*************************************************************/
#pragma once
#include "SingletonBase.h"
#define CameraMoveStartDistance 30.f		

#define CameraMaxSpeed 400.0f			//ī�޶� �ִ� ���ǵ�
#define CameraMinSpeed 100.0f			//ī�޶� �ּ� ���ǵ�
#define CameraMaxDistance 250.f			
#define CameraMinDistance 30.f 

#define CameraZoomMax 2.5f				//ī�޶� �� �ִ밪
#define CameraZoomMin 0.5f				//ī�޶� �� �ּҰ�

#define CameraDefaultStrength 2.58f
#define CameraDefaultShakeTime 0.2f
#define CameraDefaultChangeDirectionDelay 0.043f

class CameraManager : public SingletonBase<CameraManager>
{
	BlockAssign(CameraManager)
public:
	enum MoveState { None, MoveToTarget, FreeCamera, End };		//ī�޶� ���� ��
private:
	MoveState state;								//ī�޶� ����
	float zoomFactor;								//ī�޶� �� 
	Figure::FloatRect cameraRect;								//ī�޶� ��Ʈ
	Synthesize(Vector2, position, Position)			//ī�޶� ��ǥ
	class GameObject* pTarget;						//ī�޶� ���� GameObject(����ī�޶� ���� �� ���)
	Synthesize(Vector2, mapSize, MapSize)				//��ü �� ������ (ī�޶� ������ ��)
	Synthesize(float, speed, Speed)					//ī�޶� ���ǵ� ��
	Vector2 saveMouse;								//�������� ī�޶� ���(���� ������ ���콺 ��ǥ ���� ��)

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