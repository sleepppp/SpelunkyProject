#pragma once
/******************************************************************************************
## MessageComponent ##
@@ Author : ±Ç¼ø¿ì , Data : 2019.03.22
*******************************************************************************************/
class MessageComponent
{
private:
	typedef function<void(struct TagMessage)>											MessageCallback;
	typedef unordered_map<string, MessageCallback>										MessageList;
	typedef unordered_map<string, MessageCallback>::iterator							MessageIter;
private:
	MessageList mMessageList;
public:
	MessageComponent();
	virtual ~MessageComponent();

	void SendCallbackMessage(struct TagMessage message);
	void SendCallbackMessage(const string& name,const float& delayTime = 0.0f,class DataContextValue*const data = nullptr);
	void AddCallbackMessage(const string& name, function<void(struct TagMessage)> func);
};

