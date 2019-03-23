#pragma once
/*************************************************************
## Loading ##
������ �ε� Ŭ����
**************************************************************/
#include <thread>
class Loading
{
	BlockAssign(Loading)
private:
	thread* mLoadingThread;				//�ε����� ������ 
	bool mIsLoadingEnd;						//�ε��� �����°� 
	vector<function<void()>> mLoadingFuncList;		//�ε��ؾ� �� �Լ�
public: 
	Loading();
	virtual ~Loading();
	//������ �ε� ���� 
	void Start();
	//�ε��� �������� ����
	const bool& GetIsLoadEnd()const { return this->mIsLoadingEnd; }
	//�ε��ؾ� �� �Լ� �߰�
	void AddLoadFunc(function<void()> func);
private:
	//���� ���������ư� �Լ�
	void Threading();
};

