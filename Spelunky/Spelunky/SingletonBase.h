#pragma once
/****************************************************************************
## SingletonBase ##
@@ Author : 권순우 , Date : 2019.03.20

dynamicSingleton이 아니므로 직접 생성 및 해제 해주어야 함.
./System/Window/Window.h의 private CreateManager에서 해줘야함
*****************************************************************************/
template <typename T>
class SingletonBase
{
	static T* mInstance;
protected:

	SingletonBase() {};
	virtual ~SingletonBase() {};
public:
	static T* Get(void);
	static void Create(void);
	static void Delete(void);
};

template <typename T>
T* SingletonBase<T>::mInstance = nullptr;

template <typename T>
T* SingletonBase<T>::Get(void)
{
	return SingletonBase<T>::mInstance;
}

template<typename T>
inline void SingletonBase<T>::Create(void)
{
	if (SingletonBase<T>::mInstance == nullptr)
	{
		SingletonBase<T>::mInstance = new T;
	}
}

template<typename T>
inline void SingletonBase<T>::Delete(void)
{
	if (SingletonBase<T>::mInstance)
	{
		delete SingletonBase<T>::mInstance;
		SingletonBase<T>::mInstance = nullptr;
	}
}



