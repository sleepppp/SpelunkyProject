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
�ش� ������Ʈ�� �޼����� ���� ������ Ÿ���� ������ ����޼����� ������ �ٷ� ó��
@@ TagMessage message : �޼��� ����ü
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
�ش� ������Ʈ�� �޼����� ���� ������ Ÿ���� ������ ����޼����� ������ �ٷ� ó��
@@ string name : �޼��� �̸�
@@ float delayTime : �޼��� ������ Ÿ��
@@ int intData : int�� ������
@@ float floatData : float�� ������
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
�޼��� ó�� �Լ� �̸� ���
@@ string name : �޼��� �̸�
@@ function<void(TagMessge)> func : �޽��� ó�� �� �Լ�
**********************************************************************/
void MessageComponent::AddCallbackMessage(const string& name, function<void(struct TagMessage)> func)
{
	this->mMessageList.insert(make_pair(name, func));
}