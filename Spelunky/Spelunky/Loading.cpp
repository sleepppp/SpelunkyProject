#include "stdafx.h"
#include "Loading.h"

/***************************************************************
## Loading ##
@@ function<void()> func : �ε��� �Լ�
***************************************************************/
Loading::Loading()
	:mIsLoadingEnd(false), mLoadingThread(nullptr)
{
}

/***************************************************************
## ~Loading ##
������ ����ɶ����� ��� ������ �ε� Ŭ������ ������ �� 
***************************************************************/
Loading::~Loading()
{
	//������ ���������� ����Ű��
	if(mLoadingThread)
		mLoadingThread->join();
	//������� �Դٸ� �������� �����ٴ� ���̹Ƿ� �޸� �������ش�. 
	SafeDelete(mLoadingThread);
}
/***************************************************************
## Start ##
�ε��� �����Ѵ�. 
***************************************************************/
void Loading::Start()
{
	//���� �ε������尡 nullptr�̶�� �����带 �����Ѵ�. 
	//������� ������ ���ÿ� �񵿱�� ����ȴ�. 
	if (mLoadingThread == nullptr)
	{
		mLoadingThread = new thread(bind(&Loading::Threading, this));
		mIsLoadingEnd = false;
	}
}
/***************************************************************
## AddLoadFunc ##
@@ function<void()> func : �ε��ؾ� �� �Լ� 
***************************************************************/
void Loading::AddLoadFunc(function<void()> func)
{
	this->mLoadingFuncList.push_back(func);
}
/***************************************************************
## Threading ##
���������� �����尡 ���ư� �Լ� 
***************************************************************/
void Loading::Threading()
{
	//���� �ε��� �Լ��� ���� �Ѵٸ� �ش� �Լ��� �����Ѵ�.
	for (UINT i = 0; i < mLoadingFuncList.size(); ++i)
	{
		mLoadingFuncList[i]();
	}
	//������� �Դٸ� �ε��� �����ٴ� ���̹Ƿ� mIsLoadingEnd�� true�� �ٲ��ش�.
	mIsLoadingEnd = true;
	mLoadingFuncList.clear();
}

