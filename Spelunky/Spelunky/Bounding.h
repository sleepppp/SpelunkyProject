#pragma once
class Bounding
{
private:
	class GameObject* mObject; 
public:
	Bounding(class GameObject*const mObject);
	~Bounding();
	
	virtual const bool IsCollision(Figure::FloatRect*const pRect, bool isReaction = true) = 0;

	class GameObject*const GetGameObject()const { return this->mObject; }
};

