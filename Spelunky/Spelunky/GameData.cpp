#include "stdafx.h"
#include "GameData.h"

#include "DataContext.h"
GameData::GameData()
{
	for (int i = 0; i < (int)DataType::End; ++i)
	{
		mDataContainer.push_back(DataList());
	}
}


GameData::~GameData()
{
	for (UINT i = 0; i < mDataContainer.size(); ++i)
	{
		DataIter iter = mDataContainer[i].begin();
		for (; iter != mDataContainer[i].end(); ++iter)
		{			
			SafeDelete(iter->second);
		}
	}
}


