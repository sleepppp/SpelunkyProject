#pragma once
#include "Button.h"
class ImageButton final : public Button
{
private:
	class Image* mImage;
public:
	ImageButton() = delete;
	explicit ImageButton(const string& imageKey,
		const Vector2& pos,const Vector2& size, 
		const Pivot::Enum& pivot, class Transform*const pParent = nullptr);
	virtual ~ImageButton();

	void Render()final override;

	class Image*const GetImage()const { return mImage; }
	void SetImage(const string& str);
};

