#pragma once
#include "SingletonBase.h"
#include "DataContext.h"
class GameData : public SingletonBase<GameData>
{
public:
	enum class DataType : int
	{
		Int = 0 ,Float,Vector2,GameObjectPtr,String,End
	};
private:
	typedef unordered_map<string, class DataContextValue*> DataList;
	typedef unordered_map<string, class DataContextValue*>::iterator DataIter;
private:
	vector<unordered_map<string, class DataContextValue*>> mDataContainer;
public:
	GameData();
	virtual ~GameData();

	template<typename T>
	void SetData(const DataType& type,const string& key, T data)
	{
		DataContextValue** result = &mDataContainer[(int)type][key];
		if (*result == nullptr)
		{
			*result = new DataContextValue(data);
		}
		if (type == DataType::Int)
			(*result)->SetInt(reinterpret_cast<const int&>(data));
		else if (type == DataType::Float)
			(*result)->SetFloat(reinterpret_cast<const float&>(data));
		else if (type == DataType::Vector2)
			(*result)->SetVector2(reinterpret_cast<const Vector2&>(data));
		else if (type == DataType::GameObjectPtr)
			(*result)->SetGameObject(reinterpret_cast<GameObject*>(data));
		else if (type == DataType::String)
			(*result)->SetString(reinterpret_cast<const string&>(data));
	}

	DataContextValue* GetData(const DataType& type, const string& key)
	{
		return mDataContainer[(int)type][key];
	}

};

#define _GameData GameData::Get()





