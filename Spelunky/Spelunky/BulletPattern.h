#pragma once
class BulletPattern
{
private:
	class Bullet* mBullet; 
public:
	BulletPattern(class Bullet* pBullet) : mBullet(pBullet) {}
	virtual ~BulletPattern() {}
};

