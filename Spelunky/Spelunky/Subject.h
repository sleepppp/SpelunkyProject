#pragma once
class Subject final 
{
	BlockAssign(Subject)
private:
	typedef unordered_map<string, vector<class MessageComponent*>>::iterator ObserverIter;
	unordered_map<string, vector<class MessageComponent*>> mObserverList;
public:
	Subject();
	~Subject();

	void Notify(const string& name);
	void AddObserver(const string& name, class MessageComponent* pMessageModule);
	void RemoveObserver(const string& name, class MessageComponent* pMessageModule);
};

