#include "stdafx.h"
#include "CameraManager.h"

#include "GameObject.h"
#include "Transform.h"


/***************************************************************************
## CameraManager ##
***************************************************************************/
CameraManager::CameraManager()
	:mapSize((float)_WinSizeX, (float)_WinSizeY), pTarget(nullptr), zoomFactor(1.0f),
	speed(100.0f), state(CameraManager::FreeCamera), shakeDirection(1.f), moveStartDistance(CameraMoveStartDistance)
{
	this->position = Vector2(0.f, 0.f);
	this->cameraRect = Figure::RectMake(
		this->position.x, this->position.y, (float)_WinSizeX,(float) _WinSizeY);
	this->saveMouse = _Input->GetMousePosition();
	shakeChangeDelayTime = CameraDefaultChangeDirectionDelay;
}

/***************************************************************************
## ~CameraManager ##
***************************************************************************/
CameraManager::~CameraManager()
{

}

/***************************************************************************
## Update ##
ī�޶� ���¿� ���� Update
***************************************************************************/
void CameraManager::Update()
{
	//TODO Ÿ�� ī�޶��ϰ� ī�޶� ���� �߰�
	switch (state)
	{
	case CameraManager::None:
	case CameraManager::MoveToTarget:
		this->UpdateTargetCameraMode();
		break;
	case CameraManager::FreeCamera:
		this->UpdateFreeCameraMode();
		break;
	case CameraManager::End:
		this->ShakingUpdate();
		break;
	default:
		break;
	}
}


/***************************************************************************
## OnGui ##
***************************************************************************/
void CameraManager::OnGui()
{
	ImGui::Begin("Camera");
	{
		ImGui::Text("CameraPos : %d, %d", (int)position.x, (int)position.y);
		ImGui::Text("Zoom : %f", zoomFactor);
		if (ImGui::Button("ResetPos"))
		{
			this->position = Vector2(_WinSizeX / 2, _WinSizeY / 2);
			this->UpdateRenderRect();
		}
	}
	ImGui::End();
}
/***************************************************************************
## GetRelaticeRect ##
ī�޶� �����ǥ�� RECT ����
@@ RECT rc : ī�޶� ���� ���� ��Ʈ

@@ return RECT : ī�޶� ������ ��Ʈ ��ȯ
***************************************************************************/
Figure::FloatRect CameraManager::GetRelativeRect(Figure::FloatRect rc)
{
	rc.Move(Vector2(-position.x, -position.y));
	rc.left = rc.left * zoomFactor;
	rc.top = rc.top * zoomFactor;
	rc.right = rc.right * zoomFactor;
	rc.bottom = rc.bottom * zoomFactor;
	return std::move(rc);
}
/***************************************************************************
## GetRelativePoint ##
ī�޶� �����ǥ�� POINT ���� �� ��ȯ
@@ POINT pt : ���� ���� POINT

@@ return POINT : ���� �� POINT ��ȯ
***************************************************************************/
POINT CameraManager::GetRelatviePoint(POINT pt)
{
	pt.x -= (LONG)position.x;
	pt.y -= (LONG)position.y;
	pt.x = (LONG)(CastingFloat(pt.x) * zoomFactor);
	pt.y = (LONG)(CastingFloat(pt.y) * zoomFactor);
	return std::move(pt);
}

/***************************************************************************
## GetRelativeVector2 ##
ī�޶� �����ǥ�� Vector2 ���� �� ��ȯ
@@ Vector2 v : ���� ���� Vector2

@@ return Vector2 : ���� �� Vector2��ȯ
***************************************************************************/
Vector2 CameraManager::GetRelativeVector2(Vector2 v)
{
	v.x -= position.x;
	v.y -= position.y;
	v = v * zoomFactor;
	return std::move(v);
}
Vector2 CameraManager::GetWorldVector2(Vector2 v)
{
	return Vector2(v.x / zoomFactor + position.x, v.y / zoomFactor + position.y);
}
/***************************************************************************
## GetWorldMouse ##
ī�޶� ���� ���� ���콺��ǥ ��ȯ

@@ return Vector2 : ���� ������ ���콺��ǥ
***************************************************************************/
Vector2 CameraManager::GetWorldMouse()
{
	Vector2 mouse = _Input->GetMousePosition();
	return  Vector2(mouse.x / zoomFactor + position.x, mouse.y / zoomFactor + position.y);
}
/***************************************************************************
## GetWorldViewportCenter ##

@@ return Vector2 : ���� �������� ȭ�� �߽� ��ȯ
***************************************************************************/
Vector2 CameraManager::GetWorldViewportCenter()
{
	return Vector2((position.x + (float)(_WinSizeX / 2) / zoomFactor), (position.y + (float)(_WinSizeY / 2) / zoomFactor));
}

/***************************************************************************
## GetWorldMosuePoint ##

@@ return POINT : ���� �������� ���콺 ��ǥ POINT ��ȯ
***************************************************************************/
POINT CameraManager::GetWorldMousePoint()
{
	Vector2 mouse = _Input->GetMousePosition();
	mouse = Vector2(mouse.x / zoomFactor + position.x, mouse.y / zoomFactor + position.y);
	return { (LONG)mouse.x,(LONG)mouse.y };
}
/***************************************************************************
## SetZoom ##
@@ float Zoom : �� ��ġ
***************************************************************************/
void CameraManager::SetZoom(float zoom)
{
	this->zoomFactor = Math::Clampf(zoom, CameraZoomMin, CameraZoomMax);
}
/***************************************************************************
## UpdateRenderRect ##
ȭ�� Rect��ȯ
***************************************************************************/
void CameraManager::UpdateRenderRect()
{
	this->cameraRect = Figure::RectMake(
		this->position.x, this->position.y, (float)_WinSizeX,(float) _WinSizeY);
}
/***************************************************************************
## UpdateFreeCameraMode ##
FreeCaemra ������ ���� ������Ʈ
***************************************************************************/
void CameraManager::UpdateFreeCameraMode()
{
	if (IsMouseOnGui == false)
	{
		Vector2 currentMouse = _Input->GetMousePosition();
		if (_Input->GetKey(VK_RBUTTON))
		{
			if (currentMouse.x >= 0.f && currentMouse.x <= _WinSizeX &&
				currentMouse.y >= 0.f && currentMouse.y <= _WinSizeY)
			{
				if (this->saveMouse != currentMouse)
				{
					this->position += (saveMouse - currentMouse) / zoomFactor;
					this->UpdateRenderRect();
				}
			}
		}
		saveMouse = currentMouse;
	}
}
/***************************************************************************
## UpdateTargetCameraMode ##

***************************************************************************/
void CameraManager::UpdateTargetCameraMode()
{
	if (pTarget)
	{
		{
			//target�� ��ġ�� ������ �����δ�.
			Vector2 centerPos = Vector2(position.x + CastingFloat(_WinSizeX) * 0.5f, position.y + CastingFloat(_WinSizeY) * 0.5f);
			Vector2 targetPos = pTarget->GetTransform()->GetWorldPosition();
			Vector2 toTarget = targetPos - centerPos;

			switch (state)
			{
			case MoveState::None:
			{
				if (Vector2::Length(&toTarget) > moveStartDistance)
				{
					this->state = MoveState::MoveToTarget;
				}
			}
			break;

			case MoveState::MoveToTarget:
			{
				float length = Vector2::Length(&toTarget);
				this->speed = (length / (CameraMaxDistance - CameraMinDistance)) * (CameraMaxSpeed - CameraMinSpeed);
				this->speed = Math::Clampf(speed, CameraMinSpeed, CameraMaxSpeed);

				centerPos += Vector2::Normalize(&toTarget) * speed * _TimeManager->DeltaTime();
				this->position = centerPos - Vector2(_WinSizeX / 2.f, _WinSizeY / 2.f);
				this->cameraRect = Figure::RectMake(position, Vector2(_WinSizeX, _WinSizeY));

				if (Vector2::Length(&(targetPos - centerPos)) <= 50.f)
				{
					this->state = MoveState::None;
					this->position = centerPos - Vector2(_WinSizeX / 2.f, _WinSizeY / 2.f);
					this->cameraRect = Figure::RectMake(position, Vector2(_WinSizeX, _WinSizeY));
				}

				this->AmendCamera();

			}
			break;
			}
			this->ShakingUpdate();
		}

	}
}

void CameraManager::AmendCamera()
{
	if (position.x < 0.f)
	{
		position.x -= position.x;
		this->cameraRect = Figure::RectMake(position, Vector2(_WinSizeX, _WinSizeY));
	}
	else if (cameraRect.right > (LONG)mapSize.x)
	{
		position.x -= ((float)cameraRect.right) - mapSize.x;
		this->cameraRect = Figure::RectMake(position, Vector2(_WinSizeX, _WinSizeY));
	}
	if (cameraRect.top < 0)
	{
		position.y -= position.y;
		this->cameraRect = Figure::RectMake(position, Vector2(_WinSizeX, _WinSizeY));
	}
	else if (cameraRect.bottom > (LONG)mapSize.y)
	{
		position.y -= (float(cameraRect.bottom)) - mapSize.y;
		this->cameraRect = Figure::RectMake(position, Vector2(_WinSizeX, _WinSizeY));
	}
}

void CameraManager::ShakingUpdate()
{
	if (isShake == true)
	{
		shakeTime -= _TimeManager->DeltaTime();
		shakeChangeDirectionTime += _TimeManager->DeltaTime();
		if (shakeChangeDirectionTime >= shakeChangeDelayTime)
		{
			shakeDirection = -1.f * shakeDirection;
			while (shakeChangeDirectionTime >= shakeChangeDelayTime)
				shakeChangeDirectionTime -= shakeChangeDelayTime;
		}
		shakeStrength -= (shakeTime / totalShakeTime) *_TimeManager->DeltaTime();
		float strengh = shakeStrength * shakeDirection;
		position += Vector2(strengh, 0.f);
		this->cameraRect = Figure::RectMake(position, Vector2(_WinSizeX, _WinSizeY));

		if (shakeTime <= 0.f)
		{
			isShake = false;
			shakeChangeDelayTime = CameraDefaultChangeDirectionDelay;
		}
		this->AmendCamera();
	}
}

/***************************************************************************
## CameraProc ##
���콺 �ٿ� ���� ��
***************************************************************************/
void CameraManager::CameraProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (state == MoveState::FreeCamera)
	{
		if (IsMouseOnGui == false)
		{
			if ((SHORT)HIWORD(wParam) > 0)
			{
				this->zoomFactor += 0.1f;
				this->zoomFactor = Math::Clampf(zoomFactor, CameraZoomMin, CameraZoomMax);
			}
			else
			{
				this->zoomFactor -= 0.1f;
				this->zoomFactor = Math::Clampf(zoomFactor, CameraZoomMin, CameraZoomMax);
			}
		}
	}
}

void CameraManager::SetTarget(GameObject * object)
{
	if (object)
	{
		this->pTarget = object;
		this->state = MoveState::None;
		Vector2 pos = object->GetTransform()->GetWorldPosition();
		this->position = pos - Vector2(_WinSizeX / 2, _WinSizeY / 2);
		this->cameraRect = Figure::RectMake(position, Vector2(_WinSizeX, _WinSizeY));
		this->AmendCamera();
	}
}

void CameraManager::ChangeTarget(GameObject * object)
{
	pTarget = object;
	state = MoveState::None;

}

void CameraManager::SetFreeCamera()
{
	state = MoveState::FreeCamera;
}

void CameraManager::Shake(float strength, float shakeTime, float changeDirectionTime)
{
	if (isShake == false)
	{
		this->isShake = true;
		this->shakeStrength = strength;
		this->shakeTime = this->totalShakeTime = shakeTime;
		this->shakeChangeDirectionTime = 0.f;
		this->shakeChangeDelayTime = changeDirectionTime;
	}
}

void CameraManager::InitCameraToPlayer()
{
	//GameObject* object = _ObjectManager->FindObject(ObjectType::Object, "Will");
	//if (object)
	//{
	//	this->SetTarget(object);
	//	zoomFactor = 1.f;
	//}
}
