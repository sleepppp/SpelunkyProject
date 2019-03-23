#pragma once
/*************************************************************
## Loading ##
스레드 로딩 클래스
**************************************************************/
#include <thread>
class Loading
{
	BlockAssign(Loading)
private:
	thread* mLoadingThread;				//로딩해줄 쓰레드 
	bool mIsLoadingEnd;						//로딩이 끝났는가 
	vector<function<void()>> mLoadingFuncList;		//로딩해야 할 함수
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

