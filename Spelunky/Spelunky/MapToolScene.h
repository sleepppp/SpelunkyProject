#pragma once
#include "SceneBase.h"
class MapToolScene : public SceneBase
{
private:
	vector<vector<class Tile*>> mTileList;
	class ProcedureGeneration* mProcedureGen;
public:
	MapToolScene();
	virtual ~MapToolScene();

	void Init()override;
	void Release()override; 
	void Update()override; 
	void Render()override; 
};

