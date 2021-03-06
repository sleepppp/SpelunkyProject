#pragma once
class RenderPool final 
{
	BlockAssign(RenderPool)
public:
	enum class Layer : int
	{
		Background = 0,Monster, Character,Object,Tile,Bullet,Effect,UI,End
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
	const vector<class GameObject*>* GetObjectList(const Layer& layer);
	class GameObject* FindObjectInLayer(const Layer& layer, const string& name);
private:
	void ObjectRender();
	void Lighting();
	void UIRender();
	void ZOrder();
};

