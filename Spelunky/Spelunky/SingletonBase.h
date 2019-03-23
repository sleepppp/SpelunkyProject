#pragma once
/****************************************************************************
## SingletonBase ##
@@ Author : �Ǽ��� , Date : 2019.03.20

dynamicSingleton�� �ƴϹǷ� ���� ���� �� ���� ���־�� ��.
./System/Window/Window.h�� private CreateManager���� �������
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



