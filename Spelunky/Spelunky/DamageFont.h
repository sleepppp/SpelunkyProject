#pragma once
#include "GameObject.h"

class DamageFont : public GameObject 
{
private:
	class Font
	{
	public:
		wstring text; 
		Vector2 position; 
		float speed; 
		float alpha; 
		float currentTime;
		int fontSize;
		GameColor color; 

		Font(const wstring& text, const Vector2& startPos, const float& speed = 200.f, const float& alpha = 1.f,const int& size = 25, const GameColor& color
			= GameColor(1.f,1.f,1.f,1.f))
			:text(text), position(startPos), speed(speed), alpha(alpha), color(color), currentTime(0.f), fontSize(size){}

		bool Update();
		void Render();
	};
private:
	deque<Font> mDamageFontList;
public:
	DamageFont();
	virtual ~DamageFont();

	void Init()override; 
	void Release()override; 
	void Update()override; 
	void Render()override; 

	void RequestDamageFont(const wstring& text, const Vector2& pos, const float& speed = 200.f,
		const int& size = 35,const GameColor& color = GameColor(1.f, 1.f, 1.f, 1.f));
};

