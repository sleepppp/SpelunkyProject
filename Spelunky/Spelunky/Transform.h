#pragma once
/**********************************************************************
## Transform ## 
@@ Author : 권순우 , Date : 2019.03.21 

좌표 및 사이즈, 렉트 지원
각 Transform별 계층구조로 동작하게 지원(단 행렬이 없으므로 스케일 값은 안넣었음)
************************************************************************/
class Transform final
{
private:
	Vector2 mWorldPosition;
	Vector2 mLocalPosition;
	Vector2 mSize;
	Pivot::Enum mPivot; 
	Figure::FloatRect mRect; 

	Transform* mParent; 
	Transform* mFirstChild;
	Transform* mNextSibling;

	class GameObject* mGameObject;
public:
	Transform();
	Transform(const Vector2& pos, const Vector2& size, const Pivot::Enum& pivot = Pivot::LeftTop, class GameObject*const pObject = nullptr);
	virtual ~Transform();

	void AddChild(Transform*const pChild);
	void AttachTo(Transform*const pParent);
	void ReleaseParent();
	void ReleaseChild();

	void SetPivot(const Pivot::Enum& pivot);
	void SetWorldPosition(const Vector2& position);
	void SetLocalPosition(const Vector2& position);
	void SetWorldX(const float& x);
	void SetWorldY(const float& y);
	void SetLocalX(const float& x);
	void SetLocalY(const float& y);
	void SetSize(const Vector2& size);
	void SetSizeX(const float& x);
	void SetSizeY(const float& y);
	void Translate(const Vector2& moveValue);

	void Render(const D2DRenderer::DefaultBrush& brush,const bool& bRelativeCamera = false);
	void RenderHierarchy(const bool& bRelativeCamera = false);

	Vector2 GetCenterPos();
	inline Vector2 GetWorldPosition()const { return this->mWorldPosition; }
	inline Vector2 GetLocalPosition()const { return this->mLocalPosition; }
	inline Vector2 GetSize()const { return this->mSize; }
	inline Pivot::Enum GetPivot()const { return this->mPivot; }
	inline Figure::FloatRect GetRect()const { return this->mRect; }
	inline Figure::FloatRect*const GetLPRect() { return &mRect; }
	inline class GameObject*const GetGameObject()const { return this->mGameObject; }

	Transform*const GetParent()const { return this->mParent; }
	Transform*const GetFirstChild()const { return this->mFirstChild; }

	void UpdateTransform(); 
	void UpdateTransformByRect();

	void SetActiveAllChild(const bool& b);
};

