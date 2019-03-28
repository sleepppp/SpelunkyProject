#pragma once
#include "SceneBase.h"
class MapToolScene : public SceneBase
{
	BlockAssign(MapToolScene)
private:
	class MapTool* mGenerator;
public:
	MapToolScene();
	virtual ~MapToolScene();

	void Init()override; 
	void Release()override; 
	void Update()override; 
	void Render()override; 
};

