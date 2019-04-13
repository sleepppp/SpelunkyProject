#include "GameObject.h"
class PointLight : public GameObject
{
protected:
	GameColor _color;
	float _increaseCount;
	float _originRange;
	float _intensity;
	bool _isIncrease;
public:
	PointLight(Vector2 pos, float range = 460.f, float intensity = 1.25f, GameColor color = GameColor(1.f, 1.f, 1.f, 1.f));
	~PointLight();

	void Init()override;
	void Update()override;
	void Render()override;

	GameColor GetColor()const { return this->_color; }
	float GetOriginRange()const { return this->_originRange; }
	float GetIntensity()const { return this->_intensity; }
	void SetColor(const GameColor color) { this->_color = color; }
	void SetRange(const float range) { this->_originRange = range; }
	void SetIntensity(const float intensity) { this->_intensity = intensity; }
};
