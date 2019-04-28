#pragma once
#include "GameObject.h"
/******************************************************************************************
## TagMessage , DataContextValue ##
@@ Author : ±Ç¼ø¿ì , Data : 2019.03.22
*******************************************************************************************/
struct TagMessage
{
	string name;
	float delayTime;
	class DataContextValue* data;

	TagMessage()
		:name(""), delayTime(0.0f), data(nullptr) {}
	explicit TagMessage(string name, float delayTime = 0.0f, class DataContextValue* data = nullptr)
		:name(name), delayTime(delayTime), data(data) {}
};

class DataContextValue
{
	BlockAssign(DataContextValue)
private:
	void* data;
	string typeName;
public:
	string GetTypeName()const { return this->typeName; }

	DataContextValue() :data(nullptr) {}

	explicit DataContextValue(const int& value)
	{
		Initialize<int>(value);
	}
	explicit DataContextValue(const float& value)
	{
		Initialize<float>(value);
	}
	explicit DataContextValue(const string& value)
	{
		Initialize<string>(value);
	}
	explicit DataContextValue(GameObject*const value)
	{
		Initialize<GameObject*>(value);
	}
	explicit DataContextValue(const Vector2& value)
	{
		Initialize<Vector2>(value);
	}

	~DataContextValue()
	{
		if (typeName == typeid(int).name())
		{
			Release<int>();
		}
		else if (typeName == typeid(float).name())
		{
			Release<float>();
		}
		else if (typeName == typeid(string).name())
		{
			Release<string>();
		}
		else if (typeName == typeid(GameObject*).name())
		{
			Release<GameObject*>();
		}
		else if (typeName == typeid(Vector2).name())
		{
			Release<Vector2>();
		}

	}

	void SetInt(const int& value)
	{
		*static_cast<int*>(data) = value;
	}

	void SetFloat(const float& value)
	{
		*static_cast<float*>(data) = value;
	}
	void SetVector2(const Vector2& value)
	{
		*static_cast<Vector2*>(data) = value;
	}
	void SetString(const string& value)
	{
		*static_cast<string*>(data) = value;
	}
	void SetGameObject(GameObject* value)
	{
		*static_cast<GameObject**>(data) = value;
	}

	const int GetInt()
	{
		return *(static_cast<int*>(data));
	}
	const float GetFloat()
	{
		return *(static_cast<float*>(data));
	}
	const Vector2 GetVector2()
	{
		return *static_cast<Vector2*>(data);
	}
	const string GetString()
	{
		return *(static_cast<string*>(data));
	}

	GameObject*const GetGameObject()
	{
		return *(static_cast<GameObject**>(data));
	}

private:
	template <typename T>
	void Initialize(const T& value)
	{
		T* temp = new T(value);
		data = (void*)temp;
		typeName = typeid(T).name();
	}

	template <typename T>
	void Release()
	{
		if (data)
		{
			delete (static_cast<T*>(data));
			data = nullptr;
		}
	}
};
