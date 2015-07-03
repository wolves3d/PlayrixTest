#include "stdafx.h"
#include "TestWidget.h"

TestWidget::TestWidget(const std::string& name, rapidxml::xml_node<>* elem)
	: Widget(name)
	, _curTex(0)
	, _timer(0)
	, _angle(0)
	, _eff(NULL)
	, _scale(0.f)
{
	Init();
}

void TestWidget::Init()
{
	_tex1 = Core::resourceManager.Get<Render::Texture>("btnStart_Text");
	_tex2 = Core::resourceManager.Get<Render::Texture>("Circle");

	_curTex = 0;
	_angle = 0;

	m_game.Init();
}

void TestWidget::Draw()
{
	//
	// �������� ������� ��������� ������� ����.
	//
	IPoint mouse_pos = Core::mainInput.GetMousePos();

	//
	// ������������ � ���� ������� �������������� ���������, ����� � ����������
	// ����� ���� ������������ ��� �������������� ������� PopMatrix.
	//
	Render::device.PushMatrix();
	
	//
	// �������� ������� �������������� ���������, ��������� ����� ��������� � ������� ����
	// � ����������� ���������� ������������ ����� ������ ������ ��� z �� ���� _angle.
	//
	Render::device.MatrixTranslate(math::Vector3((float)mouse_pos.x, (float)mouse_pos.y, 0));
	Render::device.MatrixRotate(math::Vector3(0, 0, 1), _angle);

	if (!_curTex)
	{
		//
		// ����� Texture::Draw() ������� ���������� ������ � ��������� ��������
		// � ������ ��������� (0, 0). ����� ��������� � �������������� ��������� ������
		// ������� ��������������� � ����������� �������� ��������������.
		//
		// ��� ������ ������ Texture::Draw() �������� Texture::Bind() �������������.
		//
	//	_tex1->Draw();
	}
	else
	{
		IRect texRect = _tex2->getBitmapRect();
		//
		// ��� ��������� �������� ����� ������� �������� UV ���������� ��� ����� ��������,
		// ������� ����� �������� �� ������� �������. UV ���������� ������ ���� �������������,
		// �.�., ��� ����������� �� ������� �������� � ��������, ������ ����� ��������
		// ����� 1 (UV ���������� �������� � ��������� 0..1, ���� ����� �� ������ ������ ��
		// ������ �������, ��� ���� � ������ ������������� ��������� �������� REPEAT, ��������
		// ����� ���������� �� ���� ������� ��������������� ���������� ���).
		//

		FRect rect(texRect), uv(0, 1, 0, 1);
		_tex2->TranslateUV(rect, uv);

		Render::device.MatrixScale(_scale);
		Render::device.MatrixTranslate(math::Vector3(-texRect.width / 2.f, -texRect.height / 2.f, 0.f));

		//
		// ����������� ��������.
		//
		_tex2->Bind();
		
		//
		// ����� DrawRect() ������� � ����������� ���������� ���������� ������, ��������� �� ����
		// ���������� - �������������, ��������� ��� ���� ������� ���� ��� ������ � ����������� (binded) ��������,
		// ���� ��������� ���������������.
		//
		// ����� ������� DrawRect() ������ ���� ����� Texture::Bind(), ���� SetTexturing(false),
		// ����� ���������� ��������� ����� ���������������.
		//
		Render::DrawRect(rect, uv);
	}

	//
	// ��������������� ������� �������������� ���������, ������ �� ����� ��������� �����.
	//
	Render::device.PopMatrix();
	

	//
	// ���� ����� ��������� ��������������� ��� ���������.
	//
	Render::device.SetTexturing(false);
	
	//
	// ����� BeginColor() ������������ � ���� ������� ���� ������ � ������������� �����.
	//
	Render::BeginColor(Color(255, 128, 0, 255));
	
	//
	// ����� DrawRect() ������� � ����������� ���������� ���������� ������, ��������� �� ����
	// ���������� - �������������, ��������� ��� ���� ������� ���� ��� ������ � ����������� (binded) ��������,
	// ���� ��������� ���������������.
	//
	// ����� ������� DrawRect() ������ ���� ����� Texture::Bind(), ���� SetTexturing(false),
	// ����� ���������� ��������� ����� ���������������.
	//
	Render::DrawRect(924, 0, 100, 100);
	
	//
	// ����� EndColor() ������� �� ����� ������� ���� ������, �������������� �������.
	//
	Render::EndColor();
	
	//
	// ����� �������� ���������������.
	//
	Render::device.SetTexturing(true);

	//
	// ������ ��� �������, ������� �������� � ��������� (Update() ��� ���������� �������� �� �����).
	//
	_effCont.Draw();

	m_game.Draw();

	// debug -------------------------------------------------------------------

	static uint frameCount = 0;
	static uint secondDeadline = 0;
	static uint fps = 0;

	++frameCount;
	const uint currentTime = GetTickCount();
	if (secondDeadline == 0)
	{
		secondDeadline = (currentTime + 1000);
	}
	else if (currentTime >= secondDeadline)
	{
		fps = (10 * frameCount);
		frameCount = 0;

		secondDeadline = (currentTime + 1000);
	}

	Render::BindFont("arial");
	Render::PrintString(900 + 100 / 2, 35, utils::lexical_cast(mouse_pos.x) + ", " + utils::lexical_cast(mouse_pos.y), 1.f, LeftAlign);
	Render::PrintString(900 + 100 / 2, 15, string("FPS: ") + utils::lexical_cast(fps), 1.f, LeftAlign);
}

void TestWidget::Update(float dt)
{
	m_game.Update(dt);
	//
	// ������� ��������� � ���������
	//
	_effCont.Update(dt);

	//
	// dt - �������� ������� � ��������, ��������� �� ����������� �����.
	// ��� ����� ��������� ������ ��������, � ����������� �� ������������������ �������
	// � ��������� �����.
	//
	// ��� ����, ����� ���� �������� �������� ������ �� �������, � �� �������� ��
	// ������������������ �������, �� ������ ������������ � �� ����� ��������.
	//
	// ����������� ��� ������ � ��������� ���������.
	//
	_timer += dt * 2;
	
	//
	// ����������� ������ � ��������� (0, 2�).
	// ��� ����� ������ ��� �������������� ��������� ������������ ��������,
	// ���� ����� ������������ ��������� ����� (float ������������) ��� ��������
	// �� ����������� ����� ���� ������� ������� (������� �������-�-��).
	//
	// �������� �������� ������ ������ (0, 2�), ������ ��� �� ���������� ��� ��������
	// ��� ������� ������, � ������� �������� ����� ������ ������������ ��������� ���������.
	//
	while (_timer > 2 * math::PI)
	{
		_timer -= 2 * math::PI;
	}
	
	//
	// ������������ ��������� ��������������� � ����������� �� �������.
	//
	_scale = 0.8f + 0.25f * sinf(_timer);
}

bool TestWidget::MouseDown(const IPoint &mouse_pos)
{
	// hack
	return false;

	if (Core::mainInput.GetMouseRightButton())
	{
		//
		// ��� ������� �� ������ ������ ����, ������ ������ ������ �� ������.
		//
		_eff = _effCont.AddEffect("Iskra");
		_eff->posX = mouse_pos.x + 0.f;
		_eff->posY = mouse_pos.y + 0.f;
		_eff->Reset();
		
		//
		// � �������� ���� ������� ��������.
		//
		_angle += 25;
		while (_angle > 360)
		{
			_angle -= 360;
		}
	}
	else
	{
		//
		// ��� ������� �� ����� ������ ����, ������ ��������� ������, ������� ���������� ���.
		//
		ParticleEffect *eff = _effCont.AddEffect("FindItem2");
		eff->posX = mouse_pos.x + 0.f;
		eff->posY = mouse_pos.y + 0.f;
		eff->Reset();

		//
		// �������� �������� � 0 �� 1 � ��������.
		//
		_curTex = 1 - _curTex;
	}
	return false;
}

void TestWidget::MouseMove(const IPoint &mouse_pos)
{
	m_game.OnMouseMove();

	if (_eff)
	{
		//
		// ���� ������ ������, �� ���������� ��� � ������� ����.
		//
		_eff->posX = mouse_pos.x + 0.f;
		_eff->posY = mouse_pos.y + 0.f;
	}
}

void TestWidget::MouseUp(const IPoint &mouse_pos)
{
	if (_eff)
	{
		//
		// ���� ������ ������, �� ��� ���������� ���� ��������� ���.
		//
		_eff->Finish();
		_eff = NULL;
	}
}

void TestWidget::AcceptMessage(const Message& message)
{
	//
	// ������� ����� ���������� ��������� � �����������.
	//
	// ��������, ��� ����� ������� � ���������� �������� �� �������� ���� ����������
	// ��������� KeyPress � ����� ������� � ���������.
	//
	if (message.getPublisher() == "KeyPress") 
	{
		int code = utils::lexical_cast<int>(message.getData());
		if (code < 0)
		{
			//
			// ������������� �������� ���������� ��� ����������� ����� ������,
			// ��������, -VK_RETURN, -VK_ESCAPE � �.�.
			//
			return;
		}
		
		//
		// ������������� �������� ���������� ��� �������� ��������.
		//
		if (code == 'a')
		{
		}
	}
}
