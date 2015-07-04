#ifndef __GetReady_h_included__
#define __GetReady_h_included__


#include "BaseState.h"
#include "MainGame.h"


class CGetReady : public CBaseState
{
public:
	CGetReady()
		: m_blinkDeadline(0)
		, m_blinkCount(0)
		, m_time(0)
		, m_readyToGo(false)
	{
	}

	virtual void OnEnter()
	{
		m_blinkDeadline = -1;
		AudioWrapper::Play("GetReady");
	}

	virtual void OnUpdate(float dt)
	{
		m_time += dt;

		if (m_time > m_blinkDeadline)
		{
			m_blinkCount++;
			m_blinkDeadline = (m_time + 0.5f);

			if (true == m_readyToGo)
			{
				ExitState();
				g_Game->GetStateManager().PushState(new CMainGame());
			}
		}

		if (false == m_readyToGo)
		{
			if (m_blinkCount > 4)
			{
				m_readyToGo = true;
				g_Game->ShrinkBubbles();
			}
		}
	}

	virtual void OnDraw()
	{
		const int halfWidth = (Render::device.Width() / 2);
		const int halfHeight = (Render::device.Height() / 2);
		Render::BindFont("arial");

		if (false == m_readyToGo)
		{
			if (1 == ((int)m_blinkCount % 2))
			{
				Render::PrintString(halfWidth, halfHeight, "GET READY!", 1.f, CenterAlign);
			}
		}
		else
		{
			Render::PrintString(halfWidth, halfHeight, "GO!", 1.f, CenterAlign);
		}
	}

	float m_blinkDeadline;
	float m_blinkCount;
	float m_time;
	bool m_readyToGo;
};


#endif __GetReady_h_included__