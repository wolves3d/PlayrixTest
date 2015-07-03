#pragma once

#include "TopGunGame.h"

///
/// Виджет - основной визуальный элемент на экране.
/// Он отрисовывает себя, а также может содержать другие виджеты.
///
class TestWidget : public GUI::Widget
{
public:
	TestWidget(const std::string& name, rapidxml::xml_node<>* elem);

	void Draw();
	void Update(float dt);
	
	void AcceptMessage(const Message& message);
	
	bool MouseDown(const IPoint& mouse_pos);
	void MouseMove(const IPoint& mouse_pos);
	void MouseUp(const IPoint& mouse_pos);

private:
	void Init();

private:
	float _timer;
	
	float _scale;
	float _angle;
	
	Render::Texture *_tex1;
	Render::Texture *_tex2;
	int _curTex;

	EffectsContainer _effCont;
	ParticleEffect *_eff;

	CTopGunGame m_game;
};
