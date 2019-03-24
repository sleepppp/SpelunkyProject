#include "stdafx.h"
#include "Loading.h"

/***************************************************************
## Loading ##
@@ function<void()> func : 로딩할 함수
***************************************************************/
Loading::Loading()
	:mIsLoadingEnd(false), mLoadingThread(nullptr)
{
}

/***************************************************************
## ~Loading ##
쓰레드 종료될때까지 대기 지점은 로딩 클래스가 삭제될 때 
***************************************************************/
Loading::~Loading()
{
	//쓰레드 끝날때까지 대기시키기
	if(mLoadingThread)
		mLoadingThread->join();
	//여기까지 왔다면 쓰레딩이 끝났다는 뜻이므로 메모리 해제해준다. 
	SafeDelete(mLoadingThread);
}
/***************************************************************
## Start ##
로딩을 시작한다. 
***************************************************************/
void Loading::Start()
{
	//만약 로딩쓰레드가 nullptr이라면 쓰레드를 생성한다. 
	//쓰레드는 생성과 동시에 비동기로 실행된다. 
	if (mLoadingThread == nullptr)
	{
		mLoadingThread = new thread(bind(&Loading::Threading, this));
		mIsLoadingEnd = false;
	}
}
/***************************************************************
## AddLoadFunc ##
@@ function<void()> func : 로딩해야 할 함수 
***************************************************************/
void Loading::AddLoadFunc(function<void()> func)
{
	this->mLoadingFuncList.push_back(func);
}
/***************************************************************
## Threading ##
실질적으로 쓰레드가 돌아갈 함수 
***************************************************************/
void Loading::Threading()
{
	//만약 로딩할 함수가 존재 한다면 해당 함수를 실행한다.
	for (UINT i = 0; i < mLoadingFuncList.size(); ++i)
	{
		mLoadingFuncList[i]();
	}
	//여기까지 왔다면 로딩이 끝났다는 뜻이므로 mIsLoadingEnd값 true로 바꿔준다.
	mIsLoadingEnd = true;
	mLoadingFuncList.clear();
}

