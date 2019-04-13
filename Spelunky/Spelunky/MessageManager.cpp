#include "stdafx.h"
#include "MessageManager.h"
#include "MessageComponent.h"
#include "DataContext.h"
#include "GameObject.h"

/******************************************************************************
## MessageEvent ## 
�ش� Ÿ���� �����͵� 10���� �̸� ����
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
��ü ������ Ǯ ����
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
������ ���� �ٽ� �����´�.
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
��ü �޼��� ����
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
����� ���� �ڿ��� �ٽ� ��ȯ�Ѵ�.
@@ DataContextValue* value : ��� ���� �ڿ�
******************************************************************************/
void MessageManager::ReturnData(DataContextValue *const value)
{
	if (value == nullptr)
		return;
	this->mSleepData[value->GetTypeName()].push(value);
}
/******************************************************************************
## AddReserveMessage ##
����޼��� ���
@@ MessageModule* messageModule : �ٽ� �޽��� ���� �޼��� ���
@@ TagMessage message : �޼���
******************************************************************************/
void MessageManager::AddReserveMesseage(MessageComponent*const gameObject, TagMessage message)
{
	this->mReserveMessageList.push_back(make_pair(gameObject, message));
}
/******************************************************************************
## GetSleppData ##
�����͸� �޴´�.
@@ string type : ������ Ÿ�Գ���
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
@@ MessageComponent* pComponent : ������Ʈ 
@@ string name : �޼��� �̸� 
@@ float delayTime : �����ð� 
*********************************************************************************************************************/
void MessageManager::RequestMessage(MessageComponent * const pComponent, const string & name, const float & delayTime)
{
	pComponent->SendCallbackMessage(name, delayTime);
}
/******************************************************************************
## CreateValue ##
���������ؽ�Ʈ ����� �޴´�.
@@ string type : ������ Ÿ���̸�
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
@@ MessageComponent pComponent : ������Ʈ 
@@ TagMessage message : �޼���
*********************************************************************************************************************/
void MessageManager::RequestMessage(MessageComponent * const pComponent, const TagMessage & message)
{
	pComponent->SendCallbackMessage(message);
}

