#pragma once
/*************************************************************
## Loading ##
@@ Author : �Ǽ��� , Date : 2019.03.25
**************************************************************/
#include <thread>
class Loading
{
	BlockAssign(Loading)
private:
	thread* mLoadingThread;						
	bool mIsLoadingEnd;							
	vector<function<void()>> mLoadingFuncList;	
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

