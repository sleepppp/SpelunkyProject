#pragma once
/******************************************************************************************
## MessageManager ##
@@ Author : �Ǽ��� , Data : 2019.03.22
*******************************************************************************************/
class MessageManager
{
private:
	typedef unordered_map<string, std::stack<class DataContextValue*>>::iterator DataIter;
	typedef vector<pair<class MessageComponent*, struct TagMessage>>::iterator MessageIter;
private:
	unordered_map<string, std::stack<class DataContextValue*>> mSleepData;
	vector<pair<class MessageComponent*, struct TagMessage>> mReserveMessageList;
public:
	MessageManager();
	~MessageManager();
	void Update();
public:
	void RemoveAllMessage();
	void ReturnData(DataContextValue*const value);
	void AddReserveMesseage(class MessageComponent*const gameObject, struct TagMessage message);
public:
	template<typename T>
	DataContextValue*const GetSleepData();
	template<typename T>
	void RequestMessage(class MessageComponent* const pComponent,const string& name,const float& delayTime, const T& tData);
	void RequestMessage(class MessageComponent* const pComponent, const string& name, const float& delayTime = 0.f);
private:
	DataContextValue*const CreateValue(const string& type);
	DataContextValue*const GetSleepData(const string& type);
	void RequestMessage(MessageComponent* const pComponent, const TagMessage& message);
};

/*************************************************************************************************
## RequestMessage ## 
@@ MessageComponent* pComponent : �޼��� ������Ʈ 
@@ string name : �޼��� �̸� 
@@ float delayTime : ���� �ð� 
@@ T tData : ���ø��� ������
**************************************************************************************************/
template<typename T>
inline void MessageManager::RequestMessage(MessageComponent * const pComponent, const string & name, 
	const float & delayTime, const T & tData)
{
	DataContextValue* const value = this->GetSleepData(typeid(tData).name());
	this->RequestMessage(pComponent, TagMessage(name,delayTime,value));
}
/*************************************************************************************************
## GetSleepData ##
@@ return DataContextValue* : ���������ؽ�Ʈ
**************************************************************************************************/
template<typename T>
inline DataContextValue * const MessageManager::GetSleepData()
{
	return this->GetSleepData(typeid(T).name());
}
