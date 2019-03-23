#include "stdafx.h"
#include "MessageComponent.h"

#include "MessageManager.h"
#include "DataContext.h"

MessageComponent::MessageComponent()
{
}


MessageComponent::~MessageComponent()
{
	this->mMessageList.clear();
}
/*********************************************************************
## SendcallbackMessage ##
해당 오브젝트에 메세지를 보내 딜레이 타임이 있으면 예약메세지에 없으면 바로 처리
@@ TagMessage message : 메세지 구조체
**********************************************************************/
void MessageComponent::SendCallbackMessage(TagMessage message)
{
	if (message.delayTime > 0.0f)
	{
		//_MessageEvent->AddReserveMesseage(this, message);
	}
	else
	{
		MessageIter iter = this->mMessageList.find(message.name);
		if (iter != this->mMessageList.end())
			iter->second(message);
		//if (message.data != nullptr)
			//_MessageEvent->ReturnData(message.data);
	}
}
/*********************************************************************
## SendcallbackMessage ##
해당 오브젝트에 메세지를 보내 딜레이 타임이 있으면 예약메세지에 없으면 바로 처리
@@ string name : 메세지 이름
@@ float delayTime : 메세지 딜레이 타임
@@ int intData : int형 데이터
@@ float floatData : float형 데이터
**********************************************************************/
void MessageComponent::SendCallbackMessage(const string& name,const float& delayTime, DataContextValue*const data)
{
	if (delayTime > 0.0f)
	{
		//_MessageEvent->AddReserveMesseage(this, TagMessage(name, delayTime, data));
	}
	else
	{
		MessageIter iter = this->mMessageList.find(name);
		if (iter != this->mMessageList.end())
		{
			iter->second(TagMessage(name, delayTime, data));
		}
		//if (data != nullptr)
			//_MessageEvent->ReturnData(data);
	}
}
/*********************************************************************
## AddCallbackMessage ##
메세지 처리 함수 미리 등록
@@ string name : 메세지 이름
@@ function<void(TagMessge)> func : 메시지 처리 할 함수
**********************************************************************/
void MessageComponent::AddCallbackMessage(const string& name, function<void(struct TagMessage)> func)
{
	this->mMessageList.insert(make_pair(name, func));
}