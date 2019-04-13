#include "stdafx.h"
#include "MessageManager.h"
#include "MessageComponent.h"
#include "DataContext.h"
#include "GameObject.h"

/******************************************************************************
## MessageEvent ## 
해당 타입의 데이터들 10개씩 미리 생성
******************************************************************************/
MessageManager::MessageManager()
{
	mSleepData.insert(make_pair(typeid(int).name(), stack<DataContextValue*>()));
	mSleepData.insert(make_pair(typeid(float).name(), stack<DataContextValue*>()));
	mSleepData.insert(make_pair(typeid(string).name(), stack<DataContextValue*>()));
	mSleepData.insert(make_pair(typeid(Vector2).name(), stack<DataContextValue*>()));
	mSleepData.insert(make_pair(typeid(GameObject*).name(), stack<DataContextValue*>()));

	for (UINT i = 0; i < 10; ++i)
	{
		mSleepData[typeid(int).name()].push(new DataContextValue(0));
	}
	for (UINT i = 0; i < 10; ++i)
	{
		mSleepData[typeid(float).name()].push(new DataContextValue(0.f));
	}
	for (UINT i = 0; i < 10; ++i)
	{
		mSleepData[typeid(string).name()].push(new DataContextValue(""));
	}
	for (UINT i = 0; i < 10; ++i)
	{
		mSleepData[typeid(Vector2).name()].push(new DataContextValue(Vector2()));
	}
	for (UINT i = 0; i < 10; ++i)
	{
		mSleepData[typeid(GameObject*).name()].push(new DataContextValue(nullptr));
	}
}
/******************************************************************************
## ~MessageEvent ##
전체 데이터 풀 삭제
******************************************************************************/
MessageManager::~MessageManager()
{
	DataIter iter = mSleepData.begin();
	for (; iter != mSleepData.end(); ++iter)
	{
		UINT size = iter->second.size();
		for (UINT i = 0; i < size; ++i)
		{
			SafeDelete(iter->second.top());
			iter->second.pop();
		}
	}
	mSleepData.clear();
}
/******************************************************************************
## Release ##
데이터 전부 다시 가져온다.
******************************************************************************/
void MessageManager::Release()
{
	for (UINT i = 0; i < mReserveMessageList.size(); ++i)
	{
		this->ReturnData(mReserveMessageList[i].second.data);
	}
	mReserveMessageList.clear();
}
/******************************************************************************
## Update ##
******************************************************************************/
void MessageManager::Update()
{
	float deltaTime = _TimeManager->DeltaTime();
	MessageIter iter = this->mReserveMessageList.begin();
	for (; iter != mReserveMessageList.end(); ++iter)
	{
		iter->second.delayTime -= deltaTime;
		if (iter->second.delayTime <= 0.0f)
		{
			iter->first->SendCallbackMessage(iter->second);
			mReserveMessageList.erase(iter--);
		}
	}
}

/******************************************************************************
## RemoveAllMessage ##
전체 메세지 삭제
******************************************************************************/
void MessageManager::RemoveAllMessage()
{
	MessageIter iter = this->mReserveMessageList.begin();
	for (; iter != mReserveMessageList.end(); ++iter)
	{
		this->ReturnData(iter->second.data);
	}
}
/******************************************************************************
## ReturnData ##
사용이 끝난 자원은 다시 반환한다.
@@ DataContextValue* value : 사용 끝난 자원
******************************************************************************/
void MessageManager::ReturnData(DataContextValue *const value)
{
	if (value == nullptr)
		return;
	this->mSleepData[value->GetTypeName()].push(value);
}
/******************************************************************************
## AddReserveMessage ##
예약메세지 등록
@@ MessageModule* messageModule : 다시 메시지 받을 메세지 모듈
@@ TagMessage message : 메세지
******************************************************************************/
void MessageManager::AddReserveMesseage(MessageComponent*const gameObject, TagMessage message)
{
	this->mReserveMessageList.push_back(make_pair(gameObject, message));
}
/******************************************************************************
## GetSleppData ##
데이터를 받는다.
@@ string type : 데이터 타입네임
******************************************************************************/
DataContextValue *const MessageManager::GetSleepData(const string& type)
{
	DataContextValue* value = nullptr;
	if (mSleepData[type].size() != 0)
	{
		value = mSleepData[type].top();
		mSleepData[type].pop();
	}
	else
	{
		value = this->CreateValue(type);
	}

	return value;
}
/****************************************************************************************************************
## RequestMessage ## 
@@ MessageComponent* pComponent : 컴포넌트 
@@ string name : 메세지 이름 
@@ float delayTime : 지연시간 
*********************************************************************************************************************/
void MessageManager::RequestMessage(MessageComponent * const pComponent, const string & name, const float & delayTime)
{
	pComponent->SendCallbackMessage(name, delayTime);
}
/******************************************************************************
## CreateValue ##
데이터컨텍스트 밸류를 받는다.
@@ string type : 데이터 타입이름
******************************************************************************/
DataContextValue *const MessageManager::CreateValue(const string& type)
{
	DataContextValue* value = nullptr;
	if (type == typeid(int).name())
		value = new DataContextValue(0);
	else if (type == typeid(float).name())
		value = new DataContextValue(0.0f);
	else if (type == typeid(string).name())
		value = new DataContextValue("");
	else if (type == typeid(Vector2).name())
		value = new DataContextValue(Vector2());
	else if (type == typeid(GameObject*).name())
		value = new DataContextValue(nullptr);
	return value;
}
/****************************************************************************************************************
## RequestMessage ##
@@ MessageComponent pComponent : 컴포넌트 
@@ TagMessage message : 메세지
*********************************************************************************************************************/
void MessageManager::RequestMessage(MessageComponent * const pComponent, const TagMessage & message)
{
	pComponent->SendCallbackMessage(message);
}

