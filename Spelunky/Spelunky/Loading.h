#pragma once
/*************************************************************
## Loading ##
@@ Author : 권순우 , Date : 2019.03.25
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
	//스레드 로딩 시작 
	void Start();
	//로딩이 끝났는지 여부
	const bool& GetIsLoadEnd()const { return this->mIsLoadingEnd; }
	//로딩해야 할 함수 추가
	void AddLoadFunc(function<void()> func);
private:
	//실제 쓰레딩돌아갈 함수
	void Threading();
};

