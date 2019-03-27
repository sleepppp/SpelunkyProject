#pragma once
class RenderPool final 
{
	BlockAssign(RenderPool)
public:
	enum class Layer : int
	{
		Background = 0, Object,Tile,Effect,UI,End
	};
private:
	typedef unordered_map<Layer, vector<class GameObject*>>::iterator RenderIter;
	unordered_map<Layer, vector<class GameObject*>> mRenderList;

	class LightingManager* mLightManager; 

	bool mIsZorder;
private:
	friend class SceneBase;
	RenderPool();
	virtual ~RenderPool();
	void Release();
	void Render();
public:
	void RequestRender(const Layer& layer, class GameObject*const pRender);
	void RemoveRender(const Layer& layer, class GameObject*const pRender);
	void RemoveRender(const GameObject*const pRender);

	void SetZOrder(const bool& b) { mIsZorder = b; }

	class LightingManager*const GetLightManager()const { return mLightManager; }
private:
	void ObjectRender();
	void Lighting();
	void UIRender();
	void ZOrder();
};

